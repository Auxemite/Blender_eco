#include "env.hh"
#include "utils/shaderUtils.hh"
#include "graphics/graphics.hh"
#include "gui/window.hh"
#include "graphics/uniform.hh"
#include "scene/scene.hh"
#include "gui/gui.hh"
#include "gui/screenFrameBuffer.hh"
#include "graphics/grid.hh"
#include "editmode/editmodeScene.hh"

int main(int argc, char **argv) {
    GLFWwindow *window = Window::glfwWindowInit();
    if (window == nullptr)
        return -1;

    // Imgui & Window state
    Gui::initialize(window);
    WindowState windowState;
    glfwSetWindowUserPointer(window, &windowState);
    glfwSetKeyCallback(window, Window::keyCallback);

    Scene scene = Scene();
//    scene.addMesh("../data/cube.obj");
    scene.addMesh("../data/bunny.obj");
//    scene.addMesh("../data/plane.obj");
    scene.addTexture("../data/texture_test.jpg");
    scene.linkTextureToMesh(0, 1);
    scene.addMaterial();
    scene.linkMaterialToMesh(0, 0);

    ScreenFrameBuffer screenViewport(WIDTH, HEIGHT);
    VisualGrid visualGrid;
    Ray ray(scene.camera.position);
    EditModeScene editmodeScene = EditModeScene(scene);

    GLuint basicShaderProgram = Shader::createShaderProgram("../shaders/basic");
    GLuint editmodeShaderProgram = Shader::createShaderProgram("../shaders/editmode");
    Env::mainShaderProgram = basicShaderProgram;
    GLuint gridShaderProgram = Shader::createShaderProgram("../shaders/grid");
//    GLuint wireframeShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
    GLuint unicolorShaderProgram = Shader::createShaderProgram("../shaders/unicolor");
//    GLuint textureShaderProgram = Shader::createShaderProgram("../shaders/texture");
//    GLuint pbrShaderProgram = Shader::createShaderProgram("../shaders/pbr");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (Window::processInput(window, &scene, &editmodeScene) == 1)
            break;

        // New Frame & Ui Menu Bar
        Gui::newFrame();

        // Main render Pass
        screenViewport.bindTextures();
//        Graphics::drawInterfaceObject(unicolorShaderProgram, scene);
        if (Env::editmode) {
            visualGrid.draw(gridShaderProgram, &editmodeScene.camera_);
//            editmodeScene.drawMeshes(basicShaderProgram, {1.0, 1.0, 1.0});
            editmodeScene.drawSelectedMeshes(editmodeShaderProgram, {0.0, 0.0, 0.0});
        }
        else {
            visualGrid.draw(gridShaderProgram, &scene.camera);
            scene.drawMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
            glDisable(GL_DEPTH_TEST);
            scene.drawOutline(unicolorShaderProgram);
            glEnable(GL_DEPTH_TEST);
            scene.drawSelectedMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
        }

        // UI construction
        screenViewport.unbindTextures();
        screenViewport.load(&scene, &editmodeScene, &ray);
        Gui::mainGui(&scene, &editmodeScene, visualGrid);

        // UI render & swap buffer
        Gui::render();
        glfwSwapBuffers(window);
    }

    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}