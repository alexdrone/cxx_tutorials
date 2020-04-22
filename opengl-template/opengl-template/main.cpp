#include <iostream>
#include <fstream>
#include <sstream>
// glew.
#define GLEW_STATIC
#include <GL/glew.h>
// glfw.
#include <GLFW/glfw3.h>

constexpr GLint kWindowWidth = 800.f;
constexpr GLint kWindowHeight = 600.f;

#define GLAssert(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError(); x; GLAssert(GLLogCall(#x, __LINE__));

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char *function, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "opengl_error: " << error << " in " << function << ":" << line << std::endl;
    return false;
  }
  return true;
}

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
  
  // In order to draw a triangle we need to define a vertex buffer and a shader.
  // * A vertex buffer is a buffer in the GPu VRAM and used to issue a 'draw' call.
  // * A shader is a program that runs on the GPU that performs the draw command.
  // OpenGL works as a state machine, every command is contextual to the previous ones.
  GLuint buffer_id, vao_id;
  glGenVertexArrays(1, &vao_id);
  glGenBuffers(/* number of buffers */ 1, &buffer_id);
  glBindVertexArray(vao_id);

  // Every time an object is being created in OpenGL, an ID (unsigned int) is returned.
  // This IDs are used to refers to the existing objects in the openGL commands.
  
  // With 'glBindBuffer' we select the buffer created above.
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
  
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
  
  glBufferData(GL_ARRAY_BUFFER,
               buffer_size * sizeof(float),         // size in bytes.
               vertices,                            // ptr to the buffer.
               GL_STATIC_DRAW);                     // a hint to openGL on whether this array might
                                                    // change or not.
  // Now we have a vertex buffer with data but openGL has no knowldge of the memory layout of the
  // buffer (the choice to have pairs of 2d coordinates as above is completely arbitrary).
  
  // ** vertex attributes **
  // - A vertex can contain way more data than only positions (e.g normals, texture coordinates,
  // colors, bi-normals, tangents...)
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,                  // The index of the attribute for this vertex.
                        2,                  // How many component per element.
                        GL_FLOAT,           // The type of every component.
                        GL_FALSE,           // Whether this attribute is already normalized (-1, 1).
                        sizeof(float) * 2,  // Stride, the size of every element.
                        0);                 // Offset of the attribute.
  // To enable a vertex attribute array.
  
  // ** index buffers **
  // Everything is drawn composing triangles.
  // Given this two triangles
  // first triangle.
  // -0.5f, -0.5f,
  //  0.5f, -0.5f,
  //  0.5f,  0.5f,
  // second triangle.
  //  0.5f,  0.5f,
  // -0.5f,  0.5f,
  // -0.5f, -0.5f,
  // We have a sub-optimal configuration because many vertices are repeated.
  // index buffers alleviate this issue.
  GLuint ibo[] = {
    0, 1, 2,
    2, 3, 0
  };
  unsigned int ibo_id; // index buffer object.
  glGenBuffers(1, &ibo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_vertices * sizeof(GLuint), ibo, GL_STATIC_DRAW);
  
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

