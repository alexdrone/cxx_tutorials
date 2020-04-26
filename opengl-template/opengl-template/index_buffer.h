#pragma once
#include "macros.h"

class IndexBuffer {
public:
  IndexBuffer(const GLuint* data, GLsizei count);
  ~IndexBuffer();
  
  void Bind() const;
  void Unbind() const;
  
  inline GLsizei GetCount() const {
    return count_;
  };

private:
  GLuint renderer_id_;
  GLsizei count_;
};
