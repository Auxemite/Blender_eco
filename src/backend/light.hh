#pragma once

#include "glm/vec3.hpp"

enum class LightType {
    PointLight,
};

class Light {
public:
    LightType lightType;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    Light()=default;
};