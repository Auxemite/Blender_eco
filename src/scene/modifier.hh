#pragma once

#include "glm/vec3.hpp"
#include "material.hh"

struct Modifier {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
    Material *material = nullptr;

    Modifier()=default;

    void clear() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = 1.0f;
        material = nullptr;
    }
};
