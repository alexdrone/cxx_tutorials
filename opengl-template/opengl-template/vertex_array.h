#pragma once
#include "macros.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray {
 public:
  VertexArray();
  ~VertexArray();
  void Bind() const;
  void Unbind() const;
  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vl);

 private:
  GLuint renderer_id_;
};
