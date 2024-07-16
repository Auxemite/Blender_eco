#version 330 core
in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 materialAttr;

void main() {
    //! ambient
    float ambientStrength = materialAttr.x;
    vec3 ambient = ambientStrength * lightColor;

    //! diffuse
    float kd = materialAttr.y;
    vec3 norm = normalize(fragNormal); // https://registry.khronos.org/OpenGL-Refpages/gl4/index.php
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(0.0, dot(norm, lightDir));
    vec3 diffuseColor = diffuse * lightColor * kd;

    //! specular
    float ks = materialAttr.z;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 64);
    vec3 specular = ks * spec * lightColor ;

    vec3 res = (ambient + diffuseColor + specular) * fragColor;
    FragColor = vec4(res, 1.0);
}