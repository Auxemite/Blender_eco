#include "uniform.hh"
#include "utils/mathUtils.hh"
#include "glad/gl.h"

namespace Uniform {

void setModelViewProj(unsigned int shaderProgram, const Camera& camera) {
    Env::view = glm::lookAt(camera.position(), glm::vec3(0.0f), camera.up());

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Env::model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Env::view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Env::projection));
}

//void setModelViewProjGui(unsigned int shaderProgram, const Camera& camera) {
//    glm::vec3 interfaceCameraPosition;
//    interfaceCameraPosition.x = 30.0f * cos(glm::radians(camera->yaw_));
//    interfaceCameraPosition.z = 30.0f * sin(glm::radians(camera->yaw_));
//    interfaceCameraPosition.y = 10.0f;
//    Env::view = glm::lookAt(interfaceCameraPosition, glm::vec3(0.0f), camera.up());
//
//    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
//    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
//    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
//
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Env::model));
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Env::view));
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Env::projection));
//}

void setBasicUniforms(unsigned int shaderProgram, const Camera& camera) {
    setModelViewProj(shaderProgram, camera);

    GLint cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    glm::vec3 cameraPos = camera.position();
    glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
}

void setModifierUniforms(unsigned int shaderProgram, const Modifier &modifier) {
    GLint modifierPositionLoc = glGetUniformLocation(shaderProgram, "modifierPosition");
    glUniform3f(modifierPositionLoc, modifier.position.x, modifier.position.y, modifier.position.z);

    GLint modifierRotationLoc = glGetUniformLocation(shaderProgram, "modifierRotation");
    glm::mat3 rotationMatrix = Math::getRotationMatrix(modifier.rotation);
    glUniformMatrix3fv(modifierRotationLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

    GLint modifierScaleLoc = glGetUniformLocation(shaderProgram, "modifierScale");
    glUniform1f(modifierScaleLoc, modifier.scale);
}

void setMaterialAndTextureUniforms(unsigned int shaderProgram, const Mesh& mesh) {
    GLint modifierColorLoc = glGetUniformLocation(shaderProgram, "materialAlbedo");
    const glm::vec3 color = mesh.albedo();
    glUniform3f(modifierColorLoc, color.x, color.y, color.z);

    GLint modifierMaterialLoc = glGetUniformLocation(shaderProgram, "pbrFactor");
    const glm::vec2 pbrFactor = mesh.pbrFactor();
    glUniform2f(modifierMaterialLoc, pbrFactor.x, pbrFactor.y);
}

void setMeshUniforms(unsigned int shaderProgram, const Mesh& mesh) {
    GLint meshPositionLoc = glGetUniformLocation(shaderProgram, "meshCenter");
    const glm::vec3 center = mesh.center();
    glUniform3f(meshPositionLoc, center.x, center.y, center.z);
}

void setLightUniforms(unsigned int shaderProgram, const Light& light) {
    GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glm::vec3 pos = light.position();
    glUniform3f(lightPositionLoc, pos.x, pos.y, pos.z);

    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glm::vec3 color = light.color();
    glUniform3f(lightColorLoc, color.x, color.y, color.z);

    GLint lightIntensityLoc = glGetUniformLocation(shaderProgram, "lightPower");
    glUniform1f(lightIntensityLoc, light.intensity());
}

void setUniqueColorUniforms(unsigned int shaderProgram, const glm::vec3 &color) {
    GLint unicolorPositionLoc = glGetUniformLocation(shaderProgram, "unicolor");
    glUniform3f(unicolorPositionLoc, color.x, color.y, color.z);
}

//void setTextureUniforms(unsigned int shaderProgram, int unit) {
//    GLint unicolorPositionLoc = glGetUniformLocation(shaderProgram, "tex");
//    glUniform1i(unicolorPositionLoc, unit);
//}

}