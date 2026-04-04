#pragma once

#include "glm/vec3.hpp"

enum class LightType {
    PointLight,
};

class Light {
    public:
        LightType lightType_;
        glm::vec3 position_;
        glm::vec3 color_;
        float intensity_;

        Light()=default;
        Light(LightType lightType, glm::vec3 position, glm::vec3 color, float intensity);

        void colorModulator();
        void positionModulator();
        void intensityModulator();
};