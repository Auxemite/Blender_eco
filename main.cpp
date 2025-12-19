#include "frontend/env.hh"
#include "frontend/shaderUtils.hh"
#include "frontend/graphics.hh"
#include "frontend/window.hh"
#include "frontend/uniform.hh"
#include "backend/scene.hh"
#include "frontend/gui.hh"

glm::vec3 getMouseRay(Camera *camera, double mouseX, double mouseY,
                      int windowWidth, int windowHeight)
{
    Env::view = glm::lookAt(camera->position, glm::vec3(0.0f), camera->up);

    // Normalized Device Coordinates
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
    float z = 1.0f;  // direction vers l'avant

    // Clip space
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    // Eye space (inverse projection)
    glm::vec4 rayEye = glm::inverse(Env::projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // World space (inverse view)
    glm::vec4 rayWorld4 = glm::inverse(Env::view) * rayEye;
    glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));

    return rayWorld;
}

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
            glm::vec3 ray = getMouseRay(&scene->camera, mouseX, mouseY, WIDTH, HEIGHT);
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

        for (auto mesh : scene->meshes) {
            if (mesh->is_visible) {
                glUseProgram(Env::mainShaderProgram);
                Uniform::setBasicUniforms(Env::mainShaderProgram, &scene->camera);
//                if (mesh->selected && mesh->is_visible)
                Uniform::setModifierUniforms(Env::mainShaderProgram, &scene->modifier);
                mesh->graphicsObject->draw();

                if (mesh->selected) {
                    glUseProgram(wireframeShaderProgram);
                    Uniform::setBasicUniforms(wireframeShaderProgram, &scene->camera);
                    Uniform::setModifierUniforms(Env::mainShaderProgram, &scene->modifier);
                    mesh->graphicsObject->draw();
                }
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