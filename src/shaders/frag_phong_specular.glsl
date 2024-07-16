#version 330 core
in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 materialAttr;
uniform float lightPower;

void main() {
    //! ambient
    float ns = materialAttr[0];
    vec3 ambientColor = vec3(ns) * fragColor;

    //! diffuse
    float kd = materialAttr[1];
    vec3 norm = normalize(fragNormal); // https://registry.khronos.org/OpenGL-Refpages/gl4/index.php
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(0.0, dot(norm, lightDir));
    vec3 diffuseColor = (diffuse * kd) * fragColor;

    //! specular
    float ks = materialAttr[2];
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 64);
    vec3 specularColor = lightColor * ks * spec * lightPower;

    vec3 res = specularColor;
    FragColor = vec4(res, 1.0);
}