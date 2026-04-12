#pragma once

#include <vector>
#include "glad/gl.h"
#include "scene/light.hh"
#include "shader/shaderStructs.hh"
#include "graphics.hh"
#include "scene/modifier.hh"

class LightBuffer : NonCopyable {
    public:
        LightBuffer();
        ~LightBuffer();

        u32 count() const;
        std::vector<Shader::PointLight> getShaderLights() const;
        Light *getModifyingLight() const;

        void createVBO(const std::vector<Shader::PointLight>& shaderLights);
        void updateAndReallocVBO();
        void updateVBO();
        void bind(int index);

        void addLight(glm::vec3 position, glm::vec3 color, float intensity);
        void deleteLight(int index);

        void lightModulator();

    private:
        GLuint id_ = 0;
        std::vector<Light *> lights_;
        std::vector<const char*> lightsNames_;

        int modifierLightIndex_ = -1;
        u32 GLUsage_{};
        size_t vboSize_{};
};