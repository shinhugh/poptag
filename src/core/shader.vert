#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_texture;

out vec4 vertex_color;
out vec2 texture_coord;

void main() {

  gl_Position = vec4(in_pos, 1.0);
  vertex_color = in_color;
  texture_coord = in_texture;

}
