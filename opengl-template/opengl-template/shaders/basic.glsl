#shader:vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coords;

// varying.
// sending data from/to shaders.
// we use it to send the texture coordinates to the fragment shader.
out vec2 v_texture_coords;

// model view projection matrix.
uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * position;
  v_texture_coords = texture_coords;
}

#shader:fragment
#version 330 core
layout(location = 0) out vec4 color;

// varying from the vertex shader.
in vec2 v_texture_coords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
  vec4 texture_color = texture(u_Texture, v_texture_coords);
  color = texture_color;
}
