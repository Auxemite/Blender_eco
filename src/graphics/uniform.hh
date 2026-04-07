#pragma once

#include "env.hh"

#include "scene/camera.hh"
#include "scene/modifier.hh"
#include "scene/mesh.hh"
#include "scene/light.hh"

namespace Uniform {

    void setModelViewProj(unsigned int shaderProgram, const Camera& camera);
//    void setModelViewProjGui(unsigned int shaderProgram, const Camera& camera);
    void setBasicUniforms(unsigned int shaderProgram, const Camera& camera);
    void setModifierUniforms(unsigned int shaderProgram, const Modifier& modifier);
    void setMeshUniforms(unsigned int shaderProgram, const Mesh& mesh);
    void setMaterialAndTextureUniforms(unsigned int shaderProgram, const Mesh& mesh);
    void setLightUniforms(unsigned int shaderProgram, const Light& light);
    void setUniqueColorUniforms(unsigned int shaderProgram, const glm::vec3 &color);
//    void setTextureUniforms(unsigned int shaderProgram, int unit);
}