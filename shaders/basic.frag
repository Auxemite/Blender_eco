#version 450 core
in vec3 outColor;
in vec2 uv;

out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(uv, 1.0) * outColor, 1.0f);
}
