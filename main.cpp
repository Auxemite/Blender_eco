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
//    scene->addMesh("../data/plane.obj");
    scene->addTexture("../data/texture_test.jpg");
    scene->linkTextureToMesh(0, 0);

    ScreenFrameBuffer screenViewport(WIDTH, HEIGHT);
    VisualGrid visualGrid;
    Ray ray(scene->camera.position);

    GLuint basicShaderProgram = Shader::createShaderProgram("../shaders/basic");
    GLuint editmodeShaderProgram = Shader::createShaderProgram("../shaders/editmode");
    Env::mainShaderProgram = basicShaderProgram;
    GLuint gridShaderProgram = Shader::createShaderProgram("../shaders/grid");
    GLuint wireframeShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
    GLuint unicolorShaderProgram = Shader::createShaderProgram("../shaders/unicolor");
    GLuint textureShaderProgram = Shader::createShaderProgram("../shaders/texture");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();
        if (Window::processInput(window, scene) == 1)
            break;

        // New Frame & Ui Menu Bar
        Gui::newFrame();

        // Main render Pass
        screenViewport.bindTextures();
        visualGrid.draw(gridShaderProgram, &scene->camera);
//        Graphics::drawInterfaceObject(unicolorShaderProgram, scene);

        if (scene->editmode) {
//            scene->drawSelectedMeshes(basicShaderProgram, {1.0, 1.0, 1.0});
            scene->drawSelectedMeshes(editmodeShaderProgram, {0.0, 0.0, 0.0});
        }
        else {
            scene->drawMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
            glDisable(GL_DEPTH_TEST);
            scene->drawOutline(unicolorShaderProgram);
            glEnable(GL_DEPTH_TEST);
            scene->drawSelectedMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
        }

        // UI construction
        screenViewport.unbindTextures();
        screenViewport.load(scene, &ray);
        Gui::mainGui(scene, visualGrid);

        // UI render & swap buffer
        Gui::render();
        glfwSwapBuffers(window);
    }

    delete scene;
    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}