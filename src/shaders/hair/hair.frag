#version 330 core

in vec3 color_frag;
out vec4 output_color;

void main() {
  output_color = vec4(color_frag, 1.0);
}
