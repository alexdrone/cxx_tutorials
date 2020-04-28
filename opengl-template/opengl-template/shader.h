#pragma once
#include "macros.h"

struct ShaderSrc {
  std::string vertex_source_;
  std::string fragmet_source_;
};

enum ShaderSrcType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

class Shader {
 public:
  Shader(const std::string& filepath);
  ~Shader();
  const Shader& Bind() const;
  void Unbind() const;

  // TODO: Add the other uniforms.
  const Shader& SetUniform4f(const std::string& name, GLfloat v1, GLfloat v2,
                             GLfloat v3, GLfloat v4) const;
  // Used for texture sampler.
  const Shader& SetUniform1i(const std::string& name, GLint i);

 private:
  mutable std::unordered_map<std::string, GLint> uniform_location_cache_;
  std::string filepath_;
  GLuint renderer_id_ = 0;
  GLint GetUniformLocation(const std::string& name) const;
  GLuint CompileShader(GLuint type, const std::string& source);
  GLuint CreateShader(const std::string& vertex_shader,
                      const std::string& fragment_shader);
  ShaderSrc ParseShader(const std::string& file);
};
