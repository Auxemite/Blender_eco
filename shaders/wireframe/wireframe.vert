#version 450 core

in vec3 aPos;
in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 positionCenter;

uniform vec3 modifierPosition;
uniform mat3 modifierRotation;
uniform float modifierScale;

void main() {
  vec3 position = modifierRotation * ((aPos - positionCenter) * 1.01 + positionCenter + modifierPosition);
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
