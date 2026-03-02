#version 450 core

in vec3 position;
in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 meshCenter;
uniform vec3 modifierPosition;
uniform mat3 modifierRotation;
uniform float modifierScale;

void main() {
  vec3 position = modifierRotation * (position - meshCenter) * modifierScale * 1.01
  + modifierPosition + meshCenter;
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
