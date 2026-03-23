#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 iuv;

out vec3 ourColor;
out vec2 uv;
out vec4 p;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    p = vec4(pos[0], pos[1], pos[2], 1.0);
    gl_Position = projection * view * model * p;
    ourColor = color;
    uv = iuv;
}
