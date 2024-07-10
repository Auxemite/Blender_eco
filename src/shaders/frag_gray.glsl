#version 330 core

in vec3 fragNormal;
out vec4 FragColor;

void main() {
    vec3 normal = normalize(fragNormal);
    float intensity = (normal.x + normal.y + normal.z) / 3.0;
    intensity = (intensity + 1.0) / 2.0;
    FragColor = vec4(vec3(intensity), 1.0);
}