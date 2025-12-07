#include "frontend/env.hh"
#include "frontend/shaderUtils.hh"
#include "frontend/graphics.hh"
#include "frontend/window.hh"
#include "frontend/uniform.hh"
#include "backend/scene.hh"
#include "frontend/gui.hh"

int maintest() {
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
//    Gui::setUpTextureViewport();
    GLuint shaderProgram = Shader::initShaderProgram("../shaders/basic.vert", "../shaders/basic.frag");
    GLuint gridShaderProgram = Shader::initShaderProgram("../shaders/grid.vert", "../shaders/grid.frag");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        Env::deltaTime = currentFrame - Env::lastFrame;
        Env::lastFrame = currentFrame;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;

        Window::processInpute(window, &scene->camera);

        Graphics::clearWindow();

        Graphics::drawGrid(gridShaderProgram, &scene->camera);

        glUseProgram(shaderProgram);
        Uniform::setUniforms(shaderProgram, &scene->camera);
        scene->meshes[0]->graphicsObject->draw();

        Gui::newFrame();

        ImGui::ShowDemoWindow();
        ImGui::Begin("Viewport");
        if (ImGui::Button("Button"))
            std::cout << "Button Pressed" << "\n";
        ImGui::End();

        Gui::render(io);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete scene;
    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}