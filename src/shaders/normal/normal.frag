#version 450 core
in vec3 fragColor;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;

void main() {
    vec3 viewDir = normalize(cameraPos);
    float diff = dot(viewDir, fragNormal) / 5;
    vec3 gray = fragColor * diff;
    // vec3 gray = vec3(diff);
//    if (diff <= 0)
//        gray = fragColor * (-diff);
        // gray = vec3(-diff);

    FragColor = vec4(vec3(0.3f) + gray, 1.0f);
    // FragColor = vec4(fragColor + gray, 1.0f);
}