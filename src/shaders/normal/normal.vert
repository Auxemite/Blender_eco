#version 450 core
in vec3 aPos;
in vec3 aColor;

out vec3 vertexColor;
out vec3 vertexPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 positionDec;
uniform vec3 positionCenter;

void main() {
    gl_Position = projection * view * model * vec4(aPos[0] + positionDec[0], aPos[1] + positionDec[1], aPos[2] + positionDec[2], 1.0f);
    vertexColor = aColor;
    // vertexPos = vec3(model * vec4(aPos[0], aPos[1], aPos[2], 1.0f));
    vertexPos = aPos + positionDec;
    normal = normalize(aPos - positionCenter);
}