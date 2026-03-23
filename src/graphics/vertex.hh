#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Engine {

    struct vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec4 tangent_bitangent_sign = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    };

}