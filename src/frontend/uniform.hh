#pragma once

#include "env.hh"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "backend/camera.hh"

namespace Uniform {

    void setModelViewProj(unsigned int shaderProgram, Camera *camera);
    void setUniforms(unsigned int shaderProgram, Camera *camera);

}