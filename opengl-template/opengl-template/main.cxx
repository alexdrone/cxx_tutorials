#include "macros.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"

constexpr GLint kWindowWidth = 800.f;
constexpr GLint kWindowHeight = 600.f;


struct ShaderSrc {
  std::string vertex_source_;
  std::string fragmet_source_;
};

enum ShaderSrcType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

static ShaderSrcType ShaderTypeForLine(const std::string& line) {
  // We use a custom delimiter for the shader file.
  // #shader:vertex for a vertex shader.
  // #shader:fragment for a fragment shader.
  if (line.find("#shader:vertex") != std::string::npos) {
      return VERTEX;
  } else if (line.find("#shader:fragment") != std::string::npos)  {
    return FRAGMENT;
  }
  return NONE;
}

// Parse a text file to get the vertex and fragment shader.
static ShaderSrc ParseShader(const std::string& file) {
  std::fstream stream(file);
  std::string line;
  std::stringstream shaders[2];
  ShaderSrcType type = NONE;
  while (getline(stream, line)) {
    const auto line_type = ShaderTypeForLine(line);
    if (line_type != NONE) {
      type = line_type;
    } else {
      shaders[type] << line << "\n";
    }
  }
  return { shaders[0].str(), shaders[1].str() };
}

static GLuint CompileShader(GLuint type, const std::string& source) {
  GLuint shader_id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader_id, 1, &src, nullptr /* length: if 'null' assume is nil terminated. */);
  glCompileShader(shader_id);
  
  // Error handling.
  int result;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int lenght;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &lenght);
    auto msg = (char *)alloca(lenght * sizeof(char)); // ** workaround to get a non-constexpr lenght
                                                      // stack allocated array.
    glGetShaderInfoLog(shader_id, lenght, &lenght, msg);
    std::cout << "Failed to compile shader." << std::endl;
    std::cout << msg << std::endl;
    return 0;
  }
  return shader_id;
}

static GLuint CreateShader(const std::string& vertex_shader, const std::string& fragment_shader) {
  GLuint program_id = glCreateProgram();
  GLuint vertex_shader_id = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  GLuint fragment_shader_id = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);
  glValidateProgram(program_id);
  // The shaders are not necessary anymore since they're compiled into a program.
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  return program_id;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  const auto window = glfwCreateWindow(
      kWindowWidth, kWindowHeight, "new", nullptr, nullptr);
  int sw, sh;
  glfwGetFramebufferSize(window, &sw, &sh);
  
  if (window == nullptr) {
    std::cout << "Failed to create GFLW window." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glewExperimental = GL_TRUE;
  
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW." << std::endl;
    return -1;
  }
  
  glViewport(0, 0, sw, sh);
  
  // 'glBufferData' is used to move data inside the vertex buffer.
  constexpr int number_of_vertices = 6;
  constexpr int number_of_coordinates_per_vertex = 2;
  constexpr int buffer_size = number_of_vertices * number_of_coordinates_per_vertex;
  float vertices[buffer_size] = {
    // first triangle.
    -0.5f, -0.5f, // vertex idx 0
     0.5f, -0.5f, // vertex idx 1
     0.5f,  0.5f, // vertex idx 2
    // second triangle.
    -0.5f,  0.5f, // vertex idx 3
  };
  
  // Initialize the VAO and set its VBO + Memory Layout.
  VertexArray vao;
  VertexBuffer vbo(vertices, number_of_vertices * number_of_coordinates_per_vertex * sizeof(float));
  
  VertexBufferLayout layout;
  layout.Push<GLfloat>(2);
  vao.AddBuffer(vbo, layout);

  // ** index buffers IBO **
  GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
  };
  IndexBuffer ibo(indices, number_of_vertices);

  // ** vertex shader **
  // - program that works on every vertex individually (on the GPU).
  
  ShaderSrc shaders_src = ParseShader("shaders/basic.glsl");
  GLuint program_id = CreateShader(shaders_src.vertex_source_, shaders_src.fragmet_source_);
  glUseProgram(program_id);
  
  // ** uniforms **
  // uniforms are a way to pass data from the CPU to a shader.
  // - uniforms are set per draw
  // - attributes are set per vertex
  
  // get the location of the uniform variable (defined in the shader).
  GLint uniform_location = glGetUniformLocation(program_id, "u_color");
  GLAssert(uniform_location != -1);
  
  GLfloat r = 0.f;
  GLfloat r_increment = 0.05f;
  
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    vao.Bind();
    vbo.Bind();
    ibo.Bind();
    
    // uniform are applied per draw call and affect all of the vertices in that draw call.
    // you can't have two different uniform values for 2 different triangles.
    GLCall(glUniform4f(uniform_location, r , 0.3f, 0.8f, 1.0f));
    {
      if (r > 1) r_increment = -0.05f; else if (r < 0) r_increment = 0.05f;
      r += r_increment;
    }
    
    // We can issue a draw command for the vertex buffer.
    // If we don't have an ibo we can call: (in this case we should have all of the vertices
    // repeated).
    // glDrawArrays(GL_TRIANGLES, /* index of the first component */ 0, number_of_vertices);
    
    // Draws the triangles using the ibo.
    // ** note ** Always use GL_UNSIGNED_INT for the indices.
    GLCall(glDrawElements(GL_TRIANGLES, number_of_vertices, GL_UNSIGNED_INT, nullptr));

    // swap front and back buffer.
    glfwSwapBuffers(window);
    
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}

