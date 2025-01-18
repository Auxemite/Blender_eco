#version 450 core
in vec3 ourColor;
in vec2 uv;

out vec4 FragColor;

float rand(ivec2 seed) {
    uint n = uint(seed.x * 15731 + seed.y * 789221);
    n = (n << 13) ^ n;
    return 1.0 - float((n * (n * n * 15731u + 789221u) + 1376312589u & 0x7FFFFFFFu)) / 2147483648.0;
}

void main() {
    vec2 newUv = uv * 10.0;
    ivec2 seed = ivec2(newUv);
    float random = rand(seed);
    if (random < 0.5)
        FragColor = vec4(uv, 0.0, 1.0);
//        FragColor = vec4(ourColor, 1.0);
    else
        discard;
}
