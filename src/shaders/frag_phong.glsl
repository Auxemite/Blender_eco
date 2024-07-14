#version 330 core
in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    //! ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //! diffuse
    vec3 norm = normalize(fragNormal); // https://registry.khronos.org/OpenGL-Refpages/gl4/index.php
    vec3 lightDir = normalize(lightPos - fragPos); // https://registry.khronos.org/OpenGL-Refpages/gl4/index.php
    float diffuse = max(0.0, dot(norm, lightDir));
    vec3 diffuseColor = diffuse * lightColor;

    //! specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 64);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 res = (ambient + diffuseColor + specular) * fragColor;
    FragColor = vec4(res, 1.0);
}