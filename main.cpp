#include "env.hh"
#include "shaderUtils.hh"
#include "graphics/graphics.hh"
#include "gui/window.hh"
#include "graphics/uniform.hh"
#include "backend/scene.hh"
#include "gui/gui.hh"
#include "gui/screenFrameBuffer.hh"
#include "graphics/grid.hh"

int main(int argc, char **argv) {
    GLFWwindow *window = Window::glfwWindowInit();
    if (window == nullptr)
        return -1;

    // Imgui & Window state
    Gui::initialize(window);
    WindowState windowState;
    glfwSetWindowUserPointer(window, &windowState);
    glfwSetKeyCallback(window, Window::keyCallback);

    Scene *scene = new Scene();
    scene->addMesh("../data/cube.obj");

    ScreenFrameBuffer screenViewport(WIDTH, HEIGHT);
    VisualGrid visualGrid;
    Ray ray(scene->camera.position);

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
        Gui::newFrame();

        // Main render Pass
        screenViewport.bindTextures();
        visualGrid.draw(gridShaderProgram, &scene->camera);
//        Graphics::drawInterfaceObject(unicolorShaderProgram, scene);

        // Wireframe
        glDisable(GL_DEPTH_TEST);
        for (auto mesh : scene->meshes) {
            if (!mesh->is_visible)
                continue;

            if (mesh->selected) {
                glUseProgram(unicolorShaderProgram);
                Uniform::setBasicUniforms(unicolorShaderProgram, &scene->camera);
                Uniform::setModifierUniforms(unicolorShaderProgram, scene->modifier);
                Uniform::setMeshUniforms(unicolorShaderProgram, mesh);
                Uniform::setUniqueColorUniforms(unicolorShaderProgram, glm::vec3(1.0, 1.0, 0.0));
                mesh->graphicsObject->draw();
            }
        }
        glEnable(GL_DEPTH_TEST);

        // Mesh
        for (auto mesh : scene->meshes) {
            if (!mesh->is_visible)
                continue;

            glUseProgram(Env::mainShaderProgram);
            Uniform::setBasicUniforms(Env::mainShaderProgram, &scene->camera);
            if (mesh->selected)
                Uniform::setModifierUniforms(Env::mainShaderProgram, scene->modifier);
            else
                Uniform::setModifierUniforms(Env::mainShaderProgram,{});
            Uniform::setMeshUniforms(Env::mainShaderProgram, mesh);
            mesh->graphicsObject->draw();
        }

        // UI construction
        screenViewport.unbindTextures();
        screenViewport.load(scene, &ray);
        Gui::mainGui(scene);

        // UI render & swap buffer
        Gui::render();
        glfwSwapBuffers(window);
    }

    delete scene;
    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}