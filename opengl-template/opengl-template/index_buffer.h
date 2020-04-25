#ifndef index_buffer_header
#define index_buffer_header

#include "macros.h"

class IndexBuffer {
public:
  IndexBuffer(const GLuint* data, GLuint count);
  ~IndexBuffer();
  
  void Bind() const;
  void Unbind() const;
  
  inline GLuint GetCount() const;

private:
  unsigned int renderer_id_;
  unsigned int count_;
};

#endif /* index_buffer_header */
