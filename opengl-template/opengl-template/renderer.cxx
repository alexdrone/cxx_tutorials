#include "renderer.h"

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer &ibo, const Shader& shader) const {
  vao.Bind();
  ibo.Bind();
  shader.Bind();
  // We can issue a draw command for the vertex buffer.
  // If we don't have an ibo we can call: (in this case we should have all of the vertices
  // repeated).
  // glDrawArrays(GL_TRIANGLES, /* index of the first component */ 0, number_of_vertices);
  
  // Draws the triangles using the ibo.
  // ** note ** Always use GL_UNSIGNED_INT for the indices.
  GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
  vao.Unbind();
  ibo.Unbind();
  shader.Unbind();
}
