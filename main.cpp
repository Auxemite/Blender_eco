#include "src/frontend/imgui/imgui.h"
#include "src/frontend/imgui/imgui_impl_glfw.h"
#include "src/frontend/imgui/imgui_impl_opengl3.h"
#include <cstdio>

//MY INCLUDES
#include "src/frontend/app.hh"
#include "src/frontend/inputs.hh"

#define WIDTH 1920
#define HEIGHT 1080

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

int main(int argc, char** argv)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Blender Eco ++", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    checkOpenGLError("Post glEnable(GL_DEPTH_TEST)");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsCustom();
    //ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //TODO CODE HERE
    auto app = App();
    IM_ASSERT(app.env.image.width != 0);

    unsigned int shaderProgram = createShaderProgram("../src/shaders/vertex_shader.glsl", "../src/shaders/fragment_shader.glsl");
    checkOpenGLError("Post shader compilation");
    app.env.load_data();
    //TODO CODE HERE

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //TODO MAIN CODE START
        app.Windows();

        ImGui::Begin("Test");
        if (ImGui::Button("Move Right")) {
            vertices[2] += 0.3;
            app.env.cleanup();
            app.env.load_data();
        }

        if (ImGui::Button("Move Left")) {
            vertices[2] -= 0.3;
            app.env.cleanup();
            app.env.load_data();
        }
        ImGui::End();
        //TODO MAIN CODE END

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
//        glClear(GL_COLOR_BUFFER_BIT);

        app.env.draw_data(shaderProgram);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    app.env.cleanup();
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}