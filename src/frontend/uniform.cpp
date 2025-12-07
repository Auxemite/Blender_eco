#include "uniform.hh"

namespace Uniform {

void setModelViewProj(unsigned int shaderProgram, Camera *camera) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(camera->position, glm::vec3(0.0f), camera->up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void setUniforms(unsigned int shaderProgram, Camera *camera) {
    setModelViewProj(shaderProgram, camera);

    camera->position.x = camera->radius * cos(glm::radians(camera->yaw));
    camera->position.z = camera->radius * sin(glm::radians(camera->yaw));
    camera->position.y = camera->height.y;

    GLint cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3f(cameraPosLoc, camera->position.x, camera->position.y, camera->position.z);
}

}