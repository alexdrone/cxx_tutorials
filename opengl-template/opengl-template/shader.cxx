#include "shader.h"

Shader::Shader(const std::string& filepath) : filepath_{filepath} {
  ShaderSrc shaders_src = ParseShader(filepath);
  renderer_id_ =
      CreateShader(shaders_src.vertex_source_, shaders_src.fragmet_source_);
}

const Shader& Shader::Bind() const {
  GLCall(glUseProgram(renderer_id_));
  return *this;
}

void Shader::Unbind() const { GLCall(glUseProgram(0)); }

const Shader& Shader::SetUniform4f(const std::string& name, GLfloat v1,
                                   GLfloat v2, GLfloat v3, GLfloat v4) const {
  GLint uniform_location = GetUniformLocation(name);
  GLCall(glUniform4f(uniform_location, v1, v2, v3, v4));
  return *this;
}

const Shader& Shader::SetUniform1i(const std::string& name, GLint i) {
  GLint uniform_location = GetUniformLocation(name);
  GLCall(glUniform1i(uniform_location, i));
  return *this;
}

const Shader& Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const {
  GLint uniform_location = GetUniformLocation(name);
  GLCall(glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &matrix[0][0]));
  return *this;
}

Shader::~Shader() { GLCall(glDeleteProgram(renderer_id_)) }

#pragma mark - Private

static ShaderSrcType ShaderTypeForLine(const std::string& line) {
  // We use a custom delimiter for the shader file.
  // #shader:vertex for a vertex shader.
  // #shader:fragment for a fragment shader.
  if (line.find("#shader:vertex") != std::string::npos) {
    return VERTEX;
  } else if (line.find("#shader:fragment") != std::string::npos) {
    return FRAGMENT;
  }
  return NONE;
}

GLint Shader::GetUniformLocation(const std::string& name) const {
  // ** uniforms **
  // uniforms are a way to pass data from the CPU to a shader.
  // - uniforms are set per draw
  // - attributes are set per vertex
  // get the location of the uniform variable (defined in the shader).
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }
  GLCall(GLint uniform_location =
             glGetUniformLocation(renderer_id_, name.c_str()));
  uniform_location_cache_[name] = uniform_location;
  return uniform_location;
}

GLuint Shader::CompileShader(GLuint type, const std::string& source) {
  GLuint shader_id = glCreateShader(type);
  const char* src = source.c_str();
  GLCall(glShaderSource(shader_id, 1, &src, nullptr));
  GLCall(glCompileShader(shader_id));

  // Error handling.
  int result;
  GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int lenght;
    GLCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &lenght));
    auto msg = (char*)alloca(
        lenght * sizeof(char));  // ** workaround to get a non-constexpr lenght
                                 // stack allocated array.
    GLCall(glGetShaderInfoLog(shader_id, lenght, &lenght, msg));
    std::cout << "Failed to compile shader." << std::endl;
    std::cout << msg << std::endl;
    return 0;
  }
  return shader_id;
}

ShaderSrc Shader::ParseShader(const std::string& file) {
  std::fstream stream(file);
  std::string line;
  std::stringstream shaders[2];
  ShaderSrcType type = NONE;
  while (getline(stream, line)) {
    const auto line_type = ShaderTypeForLine(line);
    if (line_type != NONE) {
      type = line_type;
    } else {
      shaders[type] << line << "\n";
    }
  }
  return {shaders[0].str(), shaders[1].str()};
}

GLuint Shader::CreateShader(const std::string& vertex_shader,
                            const std::string& fragment_shader) {
  GLuint program_id = glCreateProgram();
  GLuint vertex_shader_id = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  GLuint fragment_shader_id =
      CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
  GLCall(glAttachShader(program_id, vertex_shader_id));
  GLCall(glAttachShader(program_id, fragment_shader_id));
  GLCall(glLinkProgram(program_id));
  GLCall(glValidateProgram(program_id));
  // The shaders are not necessary anymore since they're compiled into a
  // program.
  GLCall(glDeleteShader(vertex_shader_id));
  GLCall(glDeleteShader(fragment_shader_id)) return program_id;
}
