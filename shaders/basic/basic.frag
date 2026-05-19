#version 450 core

in vec3 fragColor;
in vec2 uv;
in vec4 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 modifierColor;
uniform vec2 modifierMaterial;
uniform vec3 unicolor;

void main() {
    vec4 dpdx = dFdx(fragPos);
    vec4 dpdy = dFdy(fragPos);
    vec3 normal = normalize(cross(dpdx.xyz, dpdy.xyz));
    vec3 viewDir = normalize(cameraPos);
    float diff = dot(viewDir, normal) / 5;
    vec3 gray = unicolor * diff;
    FragColor = vec4(vec3(0.3f) + gray, 1.0f);
//    FragColor = vec4(vec3(uv, 1.0) * outColor, 1.0f);
}
