#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 iuv;
layout (location = 3) in vec4 tanbitan;
layout (location = 4) in vec3 color;

out vec3 geomColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 meshCenter;
uniform vec3 modifierPosition;
uniform mat3 modifierRotation;
uniform float modifierScale;

void main() {
  vec3 position = modifierRotation * (position - meshCenter) * modifierScale * 1.001
  + modifierPosition + meshCenter;
  gl_Position = projection * view * model * vec4(position, 1.0f);
  geomColor = color;
}
