#include "macros.h"

void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GLLogCall(const char *function, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "opengl_error: " << error << " in " << function << ":" << line
              << std::endl;
    return false;
  }
  return true;
}
