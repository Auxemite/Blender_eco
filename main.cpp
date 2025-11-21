#include "frontend/env.hh"
#include "frontend/shader_utils.hh"
#include "frontend/graphics_utils.hh"
#include "frontend/window.hh"
#include "frontend/uniform.hh"
#include "backend/scene.hh"

int main(int argc, char **argv) {
    GLFWwindow *window = Window::glfwWindowInit();
    if (window == nullptr)
        return -1;

    Scene *scene = new Scene();

    unsigned int shaderProgram = Shader::initShaderProgram();
    Graphics::loadData();
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        Env::deltaTime = currentFrame - Env::lastFrame;
        Env::lastFrame = currentFrame;

        Window::processInpute(window, &scene->camera);

        Graphics::clearWindow();

        glUseProgram(shaderProgram);

        Uniform::setUniforms(shaderProgram, &scene->camera);

        // Draw
        glBindVertexArray(Env::VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete scene;
    return 0;
}