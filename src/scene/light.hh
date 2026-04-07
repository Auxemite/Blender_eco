#pragma once

#include "glm/vec3.hpp"

enum class LightType {
    PointLight,
};

class Light {
    public:
        Light()=default;
        Light(LightType lightType, glm::vec3 position, glm::vec3 color, float intensity);

        glm::vec3 position() const;
        glm::vec3 color() const;
        float intensity() const;

        void colorModulator();
        void positionModulator();
        void intensityModulator();

    private:
        LightType lightType_;
        glm::vec3 position_;
        glm::vec3 color_;
        float intensity_;
};