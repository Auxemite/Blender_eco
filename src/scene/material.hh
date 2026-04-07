#pragma once

#include <string>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "imgui.h"

enum class MATERIAL_TYPE {
    PHONG,
    PBR
};

class Material {
    public:
        Material()=delete;
        explicit Material(MATERIAL_TYPE materialType);

        const char* name() const;
        glm::vec3 color() const;
        glm::vec2 pbrFactor() const;

        void colorModulator();
        void pbrFactorModulator();

    private:
        std::string name_;
        MATERIAL_TYPE materialType_;
        ImVec4 color_; // r g b a
        //    glm::vec3 phong_factor; // kd ks ns
        glm::vec2 pbrFactor_; // roughness metalness
};