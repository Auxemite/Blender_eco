#version 330 core
in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;

void main() {
    vec3 viewDir = normalize(cameraPos);
    float diff = dot(viewDir, fragNormal) / 5;
    vec3 gray = vec3(diff);
    if (diff <= 0)
        gray = vec3(-diff);
    FragColor = vec4(gray + vec3(0.3f), 1.0f);
}