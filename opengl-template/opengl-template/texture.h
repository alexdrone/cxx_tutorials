#pragma once
#include "macros.h"

class Texture {
 public:
  Texture(const std::string& filepath);
  ~Texture();
  // In openGL we can bind more than a texture at the same time.
  // slot is used for that (usually 32/8 on mobile slots are available).
  void Bind(GLuint slot = 0) const;
  void Unbind() const;

  inline GLint Width() const { return width_; }
  inline GLint Height() const { return height_; }
  inline GLint Bpp() const { return bpp_; }

 private:
  std::string filepath_;
  GLint width_ = 0;
  GLint height_ = 0;
  GLint bpp_ = 0;
  GLubyte* buffer_ = nullptr;
  GLuint renderer_id_ = 0;
};
