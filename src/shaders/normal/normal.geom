#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertexColor[];
in vec3 vertexPos[];
in vec3 normal[];

out vec3 fragColor;
out vec3 fragNormal;

uniform int smooth_shading;

void main() {
    vec3 v1 = vertexPos[1] - vertexPos[0];
    vec3 v2 = vertexPos[2] - vertexPos[0];
    vec3 normal_face = normalize(cross(v1, v2));

    fragColor = vertexColor[0];
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        if (smooth_shading == 1) {
            fragNormal = normal[i];
        } else {
            fragNormal = normal_face;
        }
        EmitVertex();
    }
    EndPrimitive();
}
