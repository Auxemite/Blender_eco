#include "src/frontend/imgui/imgui.h"
#include "src/frontend/imgui/imgui_impl_glfw.h"
#include "src/frontend/imgui/imgui_impl_opengl3.h"
#include <cstdio>

//MY INCLUDES
#include "src/frontend/app.hh"
#include "src/frontend/inputs.hh"
//#include "subtest.hh"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void ToggleFullscreen(GLFWwindow* window);

int main(int argc, char** argv) {
//    submain2();
//}/*
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        glfwTerminate();
        return 1;
    }
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    const char *glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Blender Eco ++", monitor, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    checkOpenGLError("Post glEnable(GL_DEPTH_TEST)");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsCustom();
    //ImGui::StyleColorsLight();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //TODO CODE HERE
    auto app = App();
    IM_ASSERT(app.env.image.width != 0);

    unsigned int shaderPrograms[6] = {
            createShaderProgram("../src/shaders/basic"),
            createShaderProgram("../src/shaders/normal"),
            createShaderProgram("../src/shaders/phong"),
            createShaderProgram("../src/shaders/hair"),
            createShaderProgram("../src/shaders/wave")
    };
    checkOpenGLError("Post shader compilation");
    app.env.load_grid();
    checkOpenGLError("Post Loading Data");
    last_time = static_cast<float>(glfwGetTime());
    timer_interval = 0.033;
    //TODO CODE HERE

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
            break;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            ToggleFullscreen(window);
            while (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwPollEvents();
        }
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //TODO MAIN CODE START
        app.Windows();
        //TODO MAIN CODE END

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 center = glm::vec3(0.0f);
        if (!alpha_feature) {
            app.env.cameraPos.x = radius * cos(glm::radians(yaw));
            app.env.cameraPos.z = radius * sin(glm::radians(yaw));
            app.env.cameraPos.y = cameraDec.y;
            cameraFront = glm::normalize(-app.env.cameraPos);
        }

        glm::mat4 view = glm::lookAt(app.env.cameraPos, center, cameraUp);
        if (alpha_feature)
            view = glm::lookAt(app.env.cameraPos - radius * cameraFront, center, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        if (app.env.scene.activate_grid)
            app.env.draw_grid(shaderPrograms[0], model, view, projection);

        if (app.env.scene.editmode)
            app.env.draw_data(shaderPrograms[render_mode], model, view, projection, app.env.scene.focus_index, render_mode);
        else {
            for (int i = 0; i < app.env.scene.meshes.size(); ++i) {
                if (app.env.scene.meshes[i]->watch) {
                    app.env.draw_data(shaderPrograms[render_mode], model, view, projection, i, render_mode);
                    if (fur)
                        app.env.draw_data(shaderPrograms[3], model, view, projection, i, 3);
                    checkOpenGLError("Post draw_data of mesh " + std::to_string(i));
                }
            }
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < app.env.scene.meshes.size(); ++i)
        app.env.cleanup(i);

    for (unsigned int shaderProgram: shaderPrograms)
        glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void ToggleFullscreen(GLFWwindow* window)
{
    if (isFullscreen)
        glfwSetWindowMonitor(window, nullptr, 0, 0, WIDTH, HEIGHT, 0);
    else
    {
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    isFullscreen = !isFullscreen;
}

//*/