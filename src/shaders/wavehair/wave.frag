#version 450 core

//in vec3 frag_color;
out vec4 FragColor;

//uniform vec3 cameraPos;

void main() {
    vec3 object_color = vec3(1.0, 1.0, 0.0);
    FragColor = vec4(object_color, 1.0);
}
