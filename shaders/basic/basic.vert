#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 iuv;
layout (location = 3) in vec4 tanbitan;
layout (location = 4) in vec3 color;

out vec3 fragColor;
out vec2 uv;
out vec4 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 meshCenter;
uniform vec3 modifierPosition;
uniform mat3 modifierRotation;
uniform vec3 modifierScale;

void main() {
    vec3 newPos = pos;
    if (color.b == 0) {
        newPos = modifierRotation * (pos - meshCenter) * modifierScale
                 + modifierPosition + meshCenter;
    }
    vec4 matPos = vec4(newPos, 1.0);
    gl_Position = projection * view * model * matPos;
    uv = iuv;
    fragColor = color;
    fragPos = matPos;
}
