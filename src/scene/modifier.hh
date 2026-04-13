#pragma once

#include "glm/vec3.hpp"
#include "material.hh"
#include "light.hh"

struct Modifier {
    public :
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        Modifier()=default;

        void clear();
        bool isCleared() const;

        void positionModulator();
        void rotationModulator();
        void scaleModulator();
};
