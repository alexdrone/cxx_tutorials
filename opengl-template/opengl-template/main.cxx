#include "index_buffer.h"
#include "macros.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

constexpr GLint kWindowWidth = 800.f;
constexpr GLint kWindowHeight = 800.f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  const auto window =
      glfwCreateWindow(kWindowWidth, kWindowHeight, "new", nullptr, nullptr);
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // 'glBufferData' is used to move data inside the vertex buffer.
  constexpr int number_of_vertices = 4;
  constexpr int number_of_coordinates_per_vertex = 4;
  constexpr int buffer_size =
      number_of_vertices * number_of_coordinates_per_vertex;
  float vertices[buffer_size] = {
      // first triangle.
      -0.5f, -0.5f, 0.0f, 0.0f,  // vertex idx 0 + texture coordinates
      0.5f,  -0.5f, 1.0f, 0.0f,  // vertex idx 1 + texture coordinates
      0.5f,   0.5f, 1.0f, 1.0f,  // vertex idx 2 + texture coordinates
      // second triangle.
      -0.5f,  0.5f,  0.0f, 1.0f  // vertex idx 3 + texture coordinates
  };

  // Initialize the VAO and set its VBO + Memory Layout.
  VertexArray vao;
  VertexBuffer vbo(
      vertices,
      number_of_vertices * number_of_coordinates_per_vertex * sizeof(float));

  VertexBufferLayout layout;
  // vertices.
  layout.Push<GLfloat>(2);
  // texture coordinates.
  layout.Push<GLfloat>(2);
  vao.AddBuffer(vbo, layout);

  // ** index buffers IBO **
  GLuint indices[] = {0, 1, 2, 2, 3, 0};
  constexpr int number_of_indices = 6;
  IndexBuffer ibo(indices, number_of_indices);

  // ** vertex shader **
  // - program that works on every vertex individually (on the GPU).
  Shader shader("shaders/basic.glsl");
  shader.Bind();

  Texture texture("resources/texture.png");
  GLint slot = 0;
  texture.Bind(slot);
  shader.SetUniform1i("u_Texture", slot);

  GLfloat r = 0.f;
  GLfloat r_increment = 0.05f;

  Renderer renderer;

  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();

    shader.Bind().SetUniform4f("u_Color", r, .3f, .8f, 1.f);
    {
      if (r > 1)
        r_increment = -0.05f;
      else if (r < 0)
        r_increment = 0.05f;
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
