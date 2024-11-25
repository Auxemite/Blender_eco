#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertexColor[];
in vec3 vertexPos[];

out vec3 fragColor;
out vec3 fragPos;
out vec3 fragNormal;

void main() {
    vec3 v1 = vertexPos[1] - vertexPos[0];
    vec3 v2 = vertexPos[2] - vertexPos[0];
    vec3 normal = normalize(cross(v1, v2));
    fragColor = vertexColor[0];
    fragNormal = normal;
    for(int i = 0; i < 3; i++) {
        fragPos = vertexPos[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
