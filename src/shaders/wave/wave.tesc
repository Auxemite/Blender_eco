#version 450 core

layout(vertices=3) out;
//in vec3 tessctrl_color[];
//out vec3 tesseval_color[];

uniform int surface;

void main() {
    gl_TessLevelOuter[0] = surface;
    gl_TessLevelOuter[1] = surface;
    gl_TessLevelOuter[2] = surface;
    gl_TessLevelInner[0] = surface;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Pass color to tessellation evaluation shader
//    tesseval_color[gl_InvocationID] = tessctrl_color[gl_InvocationID];
}
