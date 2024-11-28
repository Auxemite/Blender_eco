#version 450 core

out vec4 FragColor;

uniform vec3 fur_color;

void main() {
    FragColor = vec4(fur_color, 1.0);
}
