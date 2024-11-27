#version 450 core

in vec4 p;
//in vec3 frag_color;

out vec4 FragColor;

//uniform vec3 cameraPos;

void main() {
    vec4 dpdx = dFdx(p);
    vec4 dpdy = dFdy(p);
    vec3 normal = normalize(cross(dpdx.xyz, dpdy.xyz));
    vec3 lightColor = vec3(0.0, 1.0, 1.0);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 lightColor2 = vec3(0.0, 0.4, 0.4);
    vec3 lightDir2 = normalize(vec3(-1.0, -1.0, -1.0));

    vec3 object_color = vec3(1.0, 1.0, 1.0);

    vec3 diffuse = max(dot(normalize(normal), lightDir), 0.3) * lightColor;
    vec3 diffuse2 = max(dot(normalize(normal), lightDir2), 0.0) * lightColor2;
    FragColor = vec4(object_color * (diffuse + diffuse2), 1.0);
}
