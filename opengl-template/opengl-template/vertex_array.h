#ifndef vertex_array_header
#define vertex_array_header

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
  unsigned int renderer_id_;
};


#endif /* vertex_array_header */
