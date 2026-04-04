#version 450 core

in vec3 fragColor;
in vec2 uv;
in vec4 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 modifierColor;
uniform vec2 modifierMaterial;
uniform vec3 unicolor;

uniform vec2 pbrFactor;
uniform vec3 materialAlbedo;

layout(binding = 0) uniform sampler2D tex;

void main() {
    vec4 dpdx = dFdx(fragPos);
    vec4 dpdy = dFdy(fragPos);
    vec3 normal = normalize(cross(dpdx.xyz, dpdy.xyz));
    vec3 viewDir = normalize(cameraPos);
    float diff = dot(viewDir, normal);
    vec3 gray = unicolor * diff;

    vec3 texColor = texture(tex, uv).rgb * diff * materialAlbedo;
    FragColor = vec4(texColor.rgb, 1.0f);
//    FragColor = vec4(vec3(uv, 0.0), 1.0f);
}
