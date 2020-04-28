#pragma once
#include "macros.h"

class VertexBuffer {
 public:
  VertexBuffer(const void* data, GLsizei size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

 private:
  GLuint renderer_id_;
};
