#include "texture.h"

#include "libs/stb_image.h"

Texture::Texture(const std::string& filepath) : filepath_{filepath} {
  // bottom-left in openGL is {0,0} so the image must be flipped vertically.
  stbi_set_flip_vertically_on_load(true);
  buffer_ = stbi_load(filepath.c_str(), &width_, &height_, &bpp_, 4 /* RGBA */);

  GLCall(glGenTextures(1, &renderer_id_));
  GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));

  // Mandatory parameters for textures.
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, buffer_));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (buffer_) {
    stbi_image_free(buffer_);
  }
}

Texture::~Texture() { GLCall(glDeleteTextures(1, &renderer_id_)); }

void Texture::Bind(GLuint slot) const {
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));
}

void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
