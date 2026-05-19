#version 450 core
layout(triangles) in;
layout(line_strip, max_vertices = 3) out;

in vec3 geomColor[];

out vec3 fragColor;

void main() {
    for(int i = 0; i < 3; i++) {
        fragColor = geomColor[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
