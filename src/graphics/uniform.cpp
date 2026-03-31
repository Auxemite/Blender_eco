#include "uniform.hh"
#include "backend/mathUtils.hh"

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
        glm::vec3 color = modifier.material->color();
        glUniform3f(modifierColorLoc, color.x, color.y, color.z);

        GLint modifierMaterialLoc = glGetUniformLocation(shaderProgram, "modifierMaterial");
        glm::vec2 pbrFactor = modifier.material->pbrFactor();
        glUniform2f(modifierMaterialLoc, pbrFactor.x, pbrFactor.y);
    }
}

void setMaterialAndTextureUniforms(unsigned int shaderProgram, Mesh *mesh) {
    mesh->graphicsObject->texture->bind(0);

    GLint modifierColorLoc = glGetUniformLocation(shaderProgram, "materialAlbedo");
    glm::vec3 color = mesh->material ? mesh->material->color() : glm::vec3(1.0f, 1.0f, 1.0f);
    glUniform3f(modifierColorLoc, color.x, color.y, color.z);

    GLint modifierMaterialLoc = glGetUniformLocation(shaderProgram, "pbrFactor");
    glm::vec2 pbrFactor = mesh->material ? mesh->material->pbrFactor() : glm::vec2(1.0f, 0.0f);
    glUniform2f(modifierMaterialLoc, pbrFactor.x, pbrFactor.y);
}

void setMeshUniforms(unsigned int shaderProgram, Mesh *mesh) {
    GLint meshPositionLoc = glGetUniformLocation(shaderProgram, "meshCenter");
    glUniform3f(meshPositionLoc, mesh->midPoint.x, mesh->midPoint.y, mesh->midPoint.z);
}

void setLightUniforms(unsigned int shaderProgram, Light *light) {
    GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPositionLoc, light->position_.x, light->position_.y, light->position_.z);

    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColorLoc, light->color_.x, light->color_.y, light->color_.z);

    GLint lightIntensityLoc = glGetUniformLocation(shaderProgram, "lightPower");
    glUniform1f(lightIntensityLoc, light->intensity_);
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