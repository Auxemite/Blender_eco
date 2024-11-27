#version 450 core

layout(vertices=2) out;

uniform int surface;

void main() {
    gl_TessLevelOuter[0] = surface;
    gl_TessLevelOuter[1] = surface;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
