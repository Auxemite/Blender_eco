#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

enum class MATERIAL_TYPE {
    PHONG,
    PBR
};

class Material {
public:
    MATERIAL_TYPE materialType;
    glm::vec3 color; // r g b
    glm::vec3 phong_factor; // kd ks ns
    glm::vec2 pbr_factor; // roughness metalness

    Material()=default;
    explicit Material(MATERIAL_TYPE _materialType);
};