#pragma once
#include "macros.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

class Renderer {
public:
  void Clear() const;
  void Draw(const VertexArray& vao, const IndexBuffer &ibo, const Shader& shader) const;
};
