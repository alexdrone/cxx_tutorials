#include "index_buffer.h"
#include "macros.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "gui_window.h"

constexpr GLfloat kWindowWidth = 800.f;
constexpr GLfloat  kWindowHeight = 600.f;

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
  
  GuiWindow::init(window);

  glViewport(0, 0, sw, sh);
  
  // How do we control blending?
  // 1.glEnable(GL_BLEND);/glDisable(GL_BLEND) [disabled by default]
  // 2.glBlendFunc(src, dest) specify how the blending is computed.
  // 3.glBlenEquation(mode) How we combine the src and dest {GL_FUNC_ADD by default).
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) results in:
  // R_src * A_src + R_dest * (1 - A_src) = R_dest
  // G_src * A_src + G_dest * (1 - A_src) = G_dest
  // B_src * A_src + B_dest * (1 - A_src) = B_dest
  // A_src * A_src + A_dest * (1 - A_src) = A_dest
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // 'glBufferData' is used to move data inside the vertex buffer.
  constexpr int number_of_vertices = 4;
  constexpr int number_of_coordinates_per_vertex = 4;
  constexpr int buffer_size =
      number_of_vertices * number_of_coordinates_per_vertex;
  float vertices[buffer_size] = {
      // first triangle.
      100.f, 100.f, 0.0f, 0.0f,  // vertex idx 0 + texture coordinates
      200.f, 100.f, 1.0f, 0.0f,  // vertex idx 1 + texture coordinates
      200.f, 200.f, 1.0f, 1.0f,  // vertex idx 2 + texture coordinates
      // second triangle.
      100.f, 200.f, 0.0f, 1.0f   // vertex idx 3 + texture coordinates
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
  
  glm::vec3 model_matrix_translation(100.f, 100.f, 0.f);

  Renderer renderer;
  
  while (!glfwWindowShouldClose(window)) {
    // ** Projection matrix: ** Maps what the "camera" sees to NDC, taking care of aspect ratio
    // and perspective. **
    // An othographic matrix is a way to transform coordinates in the screen in a way that furter
    // away elements are not shown as smaller.
    // The vertices array get multiplies against this matrix so that every vertex would be in the
    // range (-1, 1), if a vertex is beyond that range is not rendered.
    glm::mat4 projection_matrix = glm::ortho(0.f, kWindowWidth, 0.f, kWindowHeight, -1.f, 1.f);;
    // View matrix: defines position and orientation of the "camera".
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.f), glm::vec3(100.f, 100.f, 0.f));
    // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.f), model_matrix_translation);
    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;
    shader.Bind().SetUniformMat4f("u_MVP", mvp);
    
    renderer.Clear();
    renderer.Draw(vao, ibo, shader);
    
    // Gui.
    using namespace ImGui;
    GuiWindow gui("Settings");
    SliderFloat3("model_matrix_translation", &model_matrix_translation.x, 0.0f, kWindowWidth);
    gui.Render();
    
    // swap front and back buffer.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  GuiWindow::destroy();

  glfwTerminate();
  return 0;
}
