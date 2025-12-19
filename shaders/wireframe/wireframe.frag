#version 450 core

out vec4 FragColor;

void main() {
    vec3 object_color = vec3(1.0, 1.0, 0.0);
    FragColor = vec4(object_color, 1.0);
}
