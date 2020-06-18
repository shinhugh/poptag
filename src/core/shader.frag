#version 330 core

in vec4 vertex_color;
in vec2 texture_coord;

out vec4 out_color;

uniform sampler2D texture_character;

void main() {

  out_color = texture(texture_character, texture_coord);
  if(out_color.a < 0.01) {
    discard;
  }

}
