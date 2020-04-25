#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size) {
  // In order to draw a triangle we need to define a vertex buffer and a shader.
  // * A vertex buffer is a buffer in the GPu VRAM and used to issue a 'draw' call.
  // * A shader is a program that runs on the GPU that performs the draw command.
  // OpenGL works as a state machine, every command is contextual to the previous ones.
  GLCall(glGenBuffers(/* number of buffers */ 1, &renderer_id_));
  // With 'glBindBuffer' we select the buffer created above.
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));

  // 'glBufferData' is used to move data inside the vertex buffer.
  GLCall(glBufferData(GL_ARRAY_BUFFER,
               size,              // size in bytes.
               data,              // ptr to the buffer.
               GL_STATIC_DRAW));  // a hint to openGL on whether this array might change or not.
  
  // Every time an object is being created in OpenGL, an ID (unsigned int) is returned.
  // This IDs are used to refers to the existing objects in the openGL commands.
}

void VertexBuffer::Bind() const {
  // With 'glBindBuffer' we select the buffer created above.
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer() {
  GLCall(glDeleteBuffers(1, &renderer_id_));
}
