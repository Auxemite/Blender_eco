#include "uniform.hh"
#include "backend/utils.hh"

namespace Uniform {

void setModelViewProjGui(unsigned int shaderProgram, Camera *camera) {
    glm::vec3 interfaceCameraPosition;
    interfaceCameraPosition.x = 30.0f * cos(glm::radians(camera->yaw));
    interfaceCameraPosition.z = 30.0f * sin(glm::radians(camera->yaw));
    interfaceCameraPosition.y = 10.0f;
    Env::view = glm::lookAt(interfaceCameraPosition, glm::vec3(0.0f), camera->up);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Env::model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Env::view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Env::projection));
}

void setModelViewProj(unsigned int shaderProgram, Camera *camera) {
    Env::view = glm::lookAt(camera->position, glm::vec3(0.0f), camera->up);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Env::model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Env::view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Env::projection));
}

void setBasicUniforms(unsigned int shaderProgram, Camera *camera) {
    setModelViewProj(shaderProgram, camera);

    camera->position.x = camera->radius * cos(glm::radians(camera->yaw));
    camera->position.z = camera->radius * sin(glm::radians(camera->yaw));
    camera->position.y = camera->height.y;

    GLint cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3f(cameraPosLoc, camera->position.x, camera->position.y, camera->position.z);
}

void setModifierUniforms(unsigned int shaderProgram, const Modifier &modifier) {
    GLint modifierPositionLoc = glGetUniformLocation(shaderProgram, "modifierPosition");
    glUniform3f(modifierPositionLoc, modifier.position.x, modifier.position.y, modifier.position.z);

    GLint modifierRotationLoc = glGetUniformLocation(shaderProgram, "modifierRotation");
    glm::mat3 rotationMatrix = getRotationMatrix(modifier.rotation);
    glUniformMatrix3fv(modifierRotationLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

    GLint modifierScaleLoc = glGetUniformLocation(shaderProgram, "modifierScale");
    glUniform1f(modifierScaleLoc, modifier.scale);

    if (modifier.material) {
        GLint modifierColorLoc = glGetUniformLocation(shaderProgram, "modifierColor");
        glUniform3f(modifierColorLoc, modifier.material->color.x, modifier.material->color.y,
                          modifier.material->color.z);

        GLint modifierMaterialLoc = glGetUniformLocation(shaderProgram, "modifierMaterial");
        glUniform2f(modifierMaterialLoc, modifier.material->pbr_factor.x, modifier.material->pbr_factor.y);
    }
}

void setMeshUniforms(unsigned int shaderProgram, Mesh *mesh) {
    GLint meshPositionLoc = glGetUniformLocation(shaderProgram, "meshCenter");
    glUniform3f(meshPositionLoc, mesh->midPoint.x, mesh->midPoint.y, mesh->midPoint.z);
}

void setUniqueColorUniforms(unsigned int shaderProgram, const glm::vec3 &color) {
    GLint unicolorPositionLoc = glGetUniformLocation(shaderProgram, "unicolor");
    glUniform3f(unicolorPositionLoc, color.x, color.y, color.z);
}

}