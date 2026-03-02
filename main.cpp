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
//    Gui::setUpTextureViewport();
    Env::mainShaderProgram = Shader::createShaderProgram("../shaders/basic");
    GLuint gridShaderProgram = Shader::createShaderProgram("../shaders/grid");
    GLuint wireframeShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
    GLuint unicolorShaderProgram = Shader::createShaderProgram("../shaders/unicolor");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        Env::deltaTime = currentFrame - Env::lastFrame;
        Env::lastFrame = currentFrame;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;

        if (io.MouseDown[0] && io.MouseDownDuration[0] == 0) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 ray = scene->camera.getMouseRay(mouseX, mouseY, WIDTH, HEIGHT);
//            std::cout << "Ray value : x :" << ray.x << ", y : " << ray.y << ", z : " << ray.z << "\n";

            for (auto mesh : scene->meshes) {
                if (!mesh->is_visible)
                    continue;

                if (mesh->ray_intersection(scene->camera.position, ray)) {
                    mesh->selected = !mesh->selected;
                    break;
                }
            }
        }

        Window::processInpute(window, &scene->camera);

        Graphics::clearWindow();

        Graphics::drawGrid(gridShaderProgram, &scene->camera);

        // Interface objects
//        glUseProgram(unicolorShaderProgram);
//        Uniform::setModelViewProjGui(unicolorShaderProgram, &scene->camera);
//        Uniform::setModifierUniforms(unicolorShaderProgram, &scene->modifier);
//        Uniform::setUniqueColorUniforms(unicolorShaderProgram, glm::vec3(1.0, 0.0, 0.0));
//        scene->xArrow->graphicsObject->draw();
//        Uniform::setUniqueColorUniforms(unicolorShaderProgram, glm::vec3(0.0, 1.0, 0.0));
//        scene->yArrow->graphicsObject->draw();
//        Uniform::setUniqueColorUniforms(unicolorShaderProgram, glm::vec3(0.0, 0.0, 1.0));
//        scene->zArrow->graphicsObject->draw();

        for (auto mesh : scene->meshes) {
            if (mesh->is_visible) {
                glUseProgram(Env::mainShaderProgram);
                Uniform::setBasicUniforms(Env::mainShaderProgram, &scene->camera);
//                if (mesh->selected && mesh->is_visible)
                Uniform::setModifierUniforms(Env::mainShaderProgram, &scene->modifier);
                Uniform::setMeshUniforms(Env::mainShaderProgram, mesh);
                mesh->graphicsObject->draw();
            }

            // Wireframe
            if (mesh->selected) {
                glUseProgram(wireframeShaderProgram);
                Uniform::setBasicUniforms(wireframeShaderProgram, &scene->camera);
                Uniform::setModifierUniforms(wireframeShaderProgram, &scene->modifier);
                mesh->graphicsObject->draw();
            }
        }

        Gui::newFrame();
        Gui::mainGui(scene);
        Gui::render(io);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete scene;
    Gui::shutDown();
    Window::shutDown(window);

    return 0;
}