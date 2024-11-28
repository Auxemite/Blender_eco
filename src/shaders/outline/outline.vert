#version 450 core

in vec3 aPos;
in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 positionDec;

void main() {
  vec3 position = (aPos + positionDec) * 1.01;
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
