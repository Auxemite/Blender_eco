#version 450 core

in vec3 fragColor;

out vec4 FragColor;

uniform vec3 unicolor;

void main() {
    FragColor = vec4(unicolor, 1.0);
    FragColor = vec4(fragColor, 1.0);
}
