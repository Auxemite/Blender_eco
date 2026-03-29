#pragma once

#include "env.hh"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "backend/camera.hh"
#include "backend/modifier.hh"
#include "backend/mesh.hh"
#include "texture.hh"

namespace Uniform {

    void setModelViewProj(unsigned int shaderProgram, Camera *camera);
    void setModelViewProjGui(unsigned int shaderProgram, Camera *camera);
    void setBasicUniforms(unsigned int shaderProgram, Camera *camera);
    void setModifierUniforms(unsigned int shaderProgram, const Modifier& modifier);
    void setMeshUniforms(unsigned int shaderProgram, Mesh *mesh);
    void setUniqueColorUniforms(unsigned int shaderProgram, const glm::vec3 &color);
    void setTextureUniforms(unsigned int shaderProgram, int unit);
}