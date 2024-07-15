#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;
out vec3 vertexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos[0], aPos[1], aPos[2], 1.0f);
    vertexColor = aColor;
    // vertexPos = vec3(model * vec4(aPos[0], aPos[1], aPos[2], 1.0f));
    vertexPos = aPos;
}