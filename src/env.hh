#pragma once

#include <vector>
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define WIDTH 1920
#define HEIGHT 1080

namespace Env {

    inline glm::mat4 model = glm::mat4(1.0f);
    inline glm::mat4 view = glm::mat4(1.0f);
    inline glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                   static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
                                                   0.1f, 100.0f);

    inline float lastFrame = 0.0;

    inline bool audit_bindings_before_draw = false;
    inline GLuint mainShaderProgram;
}