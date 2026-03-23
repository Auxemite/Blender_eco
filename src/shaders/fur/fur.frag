#version 450 core

in float fragStatus;
out vec4 FragColor;

uniform vec3 fur_color;

void main() {
    FragColor = vec4(fur_color * fragStatus, 1.0);
}
