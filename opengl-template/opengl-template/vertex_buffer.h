#ifndef vertex_buffer_header
#define vertex_buffer_header

#include "macros.h"

class VertexBuffer {
public:
  VertexBuffer(const void* data, size_t size);
  ~VertexBuffer();
  
  void Bind() const;
  void Unbind() const;

private:
  unsigned int renderer_id_;
};

#endif /* vertex_buffer_header */
