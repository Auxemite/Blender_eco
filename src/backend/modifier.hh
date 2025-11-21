#pragma once

#include "glm/vec3.hpp"
#include "material.hh"

class Modifier {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
    Material *material;

    Modifier()=default;
};
