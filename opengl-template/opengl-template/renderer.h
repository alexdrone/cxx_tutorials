#pragma once
#include "index_buffer.h"
#include "macros.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

class Renderer {
 public:
  void Clear() const;
  void Draw(const VertexArray& vao, const IndexBuffer& ibo,
            const Shader& shader) const;
};
