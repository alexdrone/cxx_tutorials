#include "index_buffer.h"

IndexBuffer:: IndexBuffer(const GLuint* data, GLsizei count): count_{count} {
  // ** index buffers IBO **
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
  GLCall(glGenBuffers(/* number of buffers */ 1, &renderer_id_));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}

void IndexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLsizei IndexBuffer::GetCount() const {
  return count_;
}

IndexBuffer::~IndexBuffer() {
  GLCall(glDeleteBuffers(1, &renderer_id_));
}
