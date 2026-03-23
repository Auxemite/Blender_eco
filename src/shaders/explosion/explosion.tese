#version 450 core

layout(triangles, equal_spacing, ccw) in;

out vec3 position;

void main() {
    vec3 barycentric = gl_TessCoord;
    vec4 p = barycentric.x * gl_in[0].gl_Position +
        barycentric.y * gl_in[1].gl_Position +
        barycentric.z * gl_in[2].gl_Position;

    position = vec3(p.x, p.y, p.z);
    gl_Position = p;
}