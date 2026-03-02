#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 iuv;
layout (location = 3) in vec4 tanbitan;
layout (location = 4) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 meshCenter;
uniform vec3 modifierPosition;

void main() {
    vec3 newPos = pos + modifierPosition;
    gl_Position = projection * view * model * vec4(newPos, 1.0);
}
