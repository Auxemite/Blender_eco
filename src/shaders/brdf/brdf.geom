#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertexColor[];
in vec3 vertexPos[];

// Varyings (vertex shader outputs)
out vec3 fragColor;
out vec3 vNormalWS;
out vec3 ViewDirectionWS;
out vec3 in_positionWS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main() {
    vec3 v1 = vertexPos[1] - vertexPos[0];
    vec3 v2 = vertexPos[2] - vertexPos[0];
    vec3 normal = normalize(cross(v1, v2));

    fragColor = vertexColor[0];
    for(int i = 0; i < 3; i++) {
        vec4 positionLocal = vec4(vertexPos[i], 1.0);
        gl_Position = projection * view * model * gl_in[i].gl_Position;

        vNormalWS = normalize(mat3(model) * normal);
        in_positionWS = (model * positionLocal).xyz;
        ViewDirectionWS = normalize(cameraPos - in_positionWS);

        EmitVertex();
    }
    EndPrimitive();
}