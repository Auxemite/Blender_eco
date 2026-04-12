#include "env.hh"
#include "shader/shaderUtils.hh"
#include "gui/window.hh"
#include "scene/scene.hh"
#include "gui/gui.hh"
#include "gui/screenFrameBuffer.hh"
#include "graphics/grid.hh"
#include "editmode/editmodeScene.hh"

int main(int argc, char **argv) {
    // Software Context Initialization
    GLFWwindow *window = Window::softwareContextInit();
    if (window == nullptr) return -1;

    Graphics::checkOpenGLError("Error Main : Window Context Initialization");

    Scene scene = Scene();

    Graphics::checkOpenGLError("Error Main : Scene Creation");
//    scene.addMesh("../data/cube.obj");
    scene.addMesh("../data/bunny.obj");
//    scene.addMesh("../data/plane.obj");
    scene.addTexture("../data/texture_test.jpg");
    scene.linkTextureToMesh(0, 1);
    scene.addMaterial({1.0f,1.0f,1.0f}, {0.5f,0.8f});
    scene.linkMaterialToMesh(0, 0);

    Graphics::checkOpenGLError("Error Main : Scene Modification");

    ScreenFrameBuffer screenViewport(WIDTH, HEIGHT);
    VisualGrid visualGrid;
    Ray ray(scene.camera().position());
    EditMode::EditModeRay editModeRay(scene.camera().position());
    EditMode::EditModeScene editmodeScene = EditMode::EditModeScene(scene);

    Graphics::checkOpenGLError("Error Main : Pipeline Object Creation");

    GLuint basicShaderProgram = Shader::createShaderProgram("../shaders/basic");
    GLuint editmodeShaderProgram = Shader::createShaderProgram("../shaders/editmode");
    Env::mainShaderProgram = basicShaderProgram;
    GLuint gridShaderProgram = Shader::createShaderProgram("../shaders/grid");
//    GLuint wireframeShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
    GLuint unicolorShaderProgram = Shader::createShaderProgram("../shaders/unicolor");
//    GLuint textureShaderProgram = Shader::createShaderProgram("../shaders/texture");
    GLuint pbrShaderProgram = Shader::createShaderProgram("../shaders/pbr");

    Graphics::checkOpenGLError("Error Main : Programs Creation");

    glEnable(GL_DEPTH_TEST);

    for (;;) {
        glfwPollEvents();
        if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT))
            return 1;
        if (Window::processInput(window, scene, editmodeScene) == 1)
            break;

        // New Frame & Ui Menu Bar
        Gui::newFrame();

        // Main render Pass
        screenViewport.bindTextures();
//        Graphics::drawInterfaceObject(unicolorShaderProgram, scene);
        if (Env::editmode) {
            visualGrid.draw(gridShaderProgram, editmodeScene.camera());
//            editmodeScene.drawMeshes(basicShaderProgram, {1.0, 1.0, 1.0});
            editmodeScene.drawSelectedMeshes(editmodeShaderProgram, {0.0, 0.0, 0.0});
        }
        else {
            visualGrid.draw(gridShaderProgram, scene.camera());
            scene.drawMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
            glDisable(GL_DEPTH_TEST);
            scene.drawOutline(unicolorShaderProgram);
            glEnable(GL_DEPTH_TEST);
            scene.drawSelectedMeshes(Env::mainShaderProgram, {1.0, 1.0, 1.0});
        }

        // UI construction
        screenViewport.unbindTextures();
        if (Env::editmode)
            screenViewport.loadEditMode(editmodeScene, editModeRay);
        else
            screenViewport.load(scene, ray);
        Gui::mainGui(scene, editmodeScene, visualGrid);

        // UI render & swap buffer
        Gui::render();
        glfwSwapBuffers(window);
    }

    Window::shutDown(window);

    return 0;
}