#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

const vec3 bgColor = vec3(0.2f, 0.2f, 0.2f);

void main() {
    gl_Position = projection * view * model * vec4(aPos[0], aPos[1], aPos[2], 1.0);
    float diffPos = max(1.0 - 0.01 * max(distance(cameraPos, aPos) - 40.0, 0.0), 0.0);
    ourColor = mix(bgColor, aColor, diffPos);
}
