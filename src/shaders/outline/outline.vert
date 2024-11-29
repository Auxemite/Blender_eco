#version 450 core

in vec3 aPos;
in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 positionDec;
uniform vec3 positionCenter;

void main() {
  vec3 position = (aPos - positionCenter) * 1.01 + positionDec + positionCenter;
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
