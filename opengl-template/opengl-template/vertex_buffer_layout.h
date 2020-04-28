#pragma once
#include "macros.h"
#include "vertex_buffer.h"

struct VertexBufferLayoutElement {
  // The type of every component. (GL_FLOAT/GL_UNSIGNED_INT/GL_UNSIGNED_BYTE)
  GLuint type;
  // How many component per element.
  GLuint count;
  // Whether this attribute is already normalized (-1, 1). (GL_FALSE/GL_TRUE)
  GLuint normalized;

  static size_t GetSizeOfType(GLuint type) {
    switch (type) {
      case GL_FLOAT:
        return sizeof(GLfloat);
      case GL_UNSIGNED_INT:
        return sizeof(GLuint);
      case GL_UNSIGNED_BYTE:
        return sizeof(GLbyte);
    }
    GLAssert(false);
    return 0;
  }
};

class VertexBufferLayout {
 public:
  template <typename T>
  void Push(GLsizei count) {}

  template <>
  void Push<GLfloat>(GLsizei count) {
    elements_.push_back({GL_FLOAT, static_cast<GLuint>(count), GL_FALSE});
    stride_ += VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT) * count;
  }

  template <>
  void Push<GLuint>(GLsizei count) {
    elements_.push_back(
        {GL_UNSIGNED_INT, static_cast<GLuint>(count), GL_FALSE});
    stride_ +=
        VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
  }

  template <>
  void Push<GLbyte>(GLsizei count) {
    elements_.push_back(
        {GL_UNSIGNED_BYTE, static_cast<GLuint>(count), GL_TRUE});
    stride_ +=
        VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
  }

  inline std::vector<VertexBufferLayoutElement> GetElements() const {
    return elements_;
  }

  inline GLuint GetStride() const { return stride_; }

 private:
  std::vector<VertexBufferLayoutElement> elements_;
  GLuint stride_ = 0;
};
