#version 450 core
// Attributes (vertex shader inputs)
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 vertexPos;
out vec3 vertexColor;

float pi = 3.14159265359;
uniform vec3 positionDec;

void main()
{
    vertexPos = inPosition + positionDec;
    gl_Position = vec4(inPosition + positionDec, 1.0f);
    vertexColor = inColor;
}