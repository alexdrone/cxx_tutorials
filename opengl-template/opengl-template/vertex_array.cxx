#include "vertex_array.h"

VertexArray::VertexArray() {
  // ** vertex array VAO **
  GLCall(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray() {
  GLCall(glDeleteVertexArrays(1, &renderer_id_));
}

void VertexArray::Bind() const {
  GLCall(glBindVertexArray(renderer_id_));
}

void VertexArray::Unbind() const {
  GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) {
  // Now we have a vertex buffer with data but openGL has no knowldge of the memory layout of the
  // buffer (the choice to have pairs of 2d coordinates as above is completely arbitrary).
  
  // ** vertex attributes **
  // - A vertex can contain way more data than only positions (e.g normals, texture coordinates,
  // colors, bi-normals, tangents...)
  Bind();
  buffer.Bind();
  const auto& elements = layout.GetElements();
  GLuint offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& e = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    glVertexAttribPointer(i,                      // The index of the attribute for this vertex.
                          e.count,                // How many component per element.
                          e.type,                 // The type of every component.
                          e.normalized,           // Whether this attribute is already normalized.
                          layout.GetStride(),     // Stride, the size of every element.
                          (const void *)offset);  // Offset of the attribute.

    offset += VertexBufferLayoutElement::GetSizeOfType(e.type) * e.count;
  }
}
