#include "frontend/env.hh"
#include "frontend/shaderUtils.hh"
#include "frontend/graphics.hh"
#include "frontend/window.hh"
#include "frontend/uniform.hh"
#include "backend/scene.hh"
#include "frontend/gui.hh"

int main(int argc, char **argv) {
    GLFWwindow *window = Window::glfwWindowInit();
    if (window == nullptr)
        return -1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    Gui::initialize(io, window);

    Scene *scene = new Scene();
    scene->addMesh("../data/cube.obj");
    Graphics::loadGrid();
    Gui::loadTextureViewport();
    Env::mainShaderProgram = Shader::createShaderProgram("../shaders/basic");
    GLuint gridShaderProgram = Shader::createShaderProgram("../shaders/grid");
    GLuint wireframeShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
    GLuint unicolorShaderProgram = Shader::createShaderProgram("../shaders/unicolor");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();
        if (Window::processInput(window, &scene->camera) == 1)
            break;

        // New Frame & Ui Menu Bar
        Gui::newFrame(io);

        // Main render Pass
        Graphics::bindAndClearWindow();
        Graphics::drawGrid(gridShaderProgram, &scene->camera);
//        Graphics::drawInterfaceObject(unicolorShaderProgram, scene);
        for (auto mesh : scene->meshes) {
            if (mesh->is_visible) {
                glUseProgram(Env::mainShaderProgram);
                Uniform::setBasicUniforms(Env::mainShaderProgram, &scene->camera);

                if (mesh->selected && mesh->is_visible)
                    Uniform::setModifierUniforms(Env::mainShaderProgram, scene->modifier);
                else
                    Uniform::setModifierUniforms(Env::mainShaderProgram,{});

                Uniform::setMeshUniforms(Env::mainShaderProgram, mesh);
                mesh->graphicsObject->draw();
            }

            // Wireframe
            if (mesh->selected) {
                glUseProgram(wireframeShaderProgram);
                Uniform::setBasicUniforms(wireframeShaderProgram, &scene->camera);
                Uniform::setModifierUniforms(wireframeShaderProgram, scene->modifier);
                mesh->graphicsObject->draw();
            }
        }

        // UI construction
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Gui::mainGui(scene);

        // UI render & swap buffer
        Gui::render(io);
        glfwSwapBuffers(window);
    }

    delete scene;
    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}