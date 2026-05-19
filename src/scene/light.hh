#pragma once

#include <string>
#include "glm/vec3.hpp"
#include "shader/shaderStructs.hh"

enum class LightType {
    PointLight,
};

class Light {
    public:
        Light()=delete;
        Light(LightType lightType, glm::vec3 position, glm::vec3 color, float intensity);

        const char* name() const;
        glm::vec3 position() const;
        glm::vec3 color() const;
        float intensity() const;
        Shader::PointLight getShaderStruct() const;

        void colorModulator();
        void positionModulator();
        void intensityModulator();

    private:
        std::string name_;
        LightType lightType_;
        glm::vec3 position_;
        glm::vec3 color_;
        float intensity_;
};