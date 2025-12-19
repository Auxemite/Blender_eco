#include "uniform.hh"

namespace Uniform {

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

glm::mat3 getRotationMatrix(Modifier *modifier) {
    float angleX = modifier->rotation.x;
    return {1.0, 0.0, 0.0,
            0.0, cos(angleX), -sin(angleX),
            0.0, sin(angleX), cos(angleX) };
}

void setModifierUniforms(unsigned int shaderProgram, Modifier *modifier) {
    GLint modifierPositionLoc = glGetUniformLocation(shaderProgram, "modifierPosition");
    glUniform3f(modifierPositionLoc, modifier->position.x, modifier->position.y, modifier->position.z);

    GLint modifierRotationLoc = glGetUniformLocation(shaderProgram, "modifierRotation");
    glm::mat3 rotationMatrix = getRotationMatrix(modifier);
    glUniformMatrix3fv(modifierRotationLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

    GLint modifierScaleLoc = glGetUniformLocation(shaderProgram, "modifierScale");
    glUniform1f(modifierScaleLoc, modifier->scale);

    if (modifier->material) {
        GLint modifierColorLoc = glGetUniformLocation(shaderProgram, "modifierColor");
        glUniform3f(modifierColorLoc, modifier->material->color.x, modifier->material->color.y,
                          modifier->material->color.z);

        GLint modifierMaterialLoc = glGetUniformLocation(shaderProgram, "modifierMaterial");
        glUniform2f(modifierMaterialLoc, modifier->material->pbr_factor.x, modifier->material->pbr_factor.y);
    }
}

}