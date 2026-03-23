#version 450 core

in vec3 ourColor;
in vec2 uv;
in vec4 p;

out vec4 FragColor;

uniform float density;
uniform vec3 cameraPos;

float rand(ivec2 seed) {
    uint n = uint(seed.x * 15731 + seed.y * 789221);
    n = (n << 13) ^ n;
    return 1.0 - float((n * (n * n * 15731u + 789221u) + 1376312589u & 0x7FFFFFFFu)) / 2147483648.0;
}

void main() {
    vec4 dpdx = dFdx(p);
    vec4 dpdy = dFdy(p);
    vec3 normal = normalize(cross(dpdx.xyz, dpdy.xyz));
    vec3 viewDir = normalize(cameraPos);
    float diff = dot(viewDir, normal) / 5 + 0.3;

    vec2 newUv = uv * 100.0;
    ivec2 seed = ivec2(newUv);
    float random = rand(seed);
    if (random < density) {
        vec3 color = vec3(uv, 1.0) * gl_FragCoord.y / 1000.0;
        FragColor = vec4(color, 1.0);
        //        FragColor = vec4(ourColor, 1.0);
    }
    else
        discard;
}
