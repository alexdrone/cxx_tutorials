#include "macros.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "renderer.h"

constexpr GLint kWindowWidth = 800.f;
constexpr GLint kWindowHeight = 600.f;

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
  Shader shader("shaders/basic.glsl");
  shader.Bind();

  GLfloat r = 0.f;
  GLfloat r_increment = 0.05f;
  
  Renderer renderer;
  
  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();
    
    shader.Bind().SetUniform4f("u_color", r, .3f, .8f, 1.f);
    {
      if (r > 1) r_increment = -0.05f; else if (r < 0) r_increment = 0.05f;
      r += r_increment;
    }
    renderer.Draw(vao, ibo, shader);
    
    // swap front and back buffer.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}

