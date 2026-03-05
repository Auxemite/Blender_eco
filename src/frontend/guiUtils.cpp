#include <iostream>
#include "guiUtils.hh"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"
#include "graphics.hh"

namespace Gui {

void initialize(ImGuiIO &io, GLFWwindow *window) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsCustom();
    //ImGui::StyleColorsLight();

    ImGuiStyle &style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void newFrame(ImGuiIO &io) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("Main Viewport", nullptr, window_flags);
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::End();
}

void render(ImGuiIO &io) {
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void shutDown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void loadTextureViewport() {
    // Create main color texture
    mainColorTexture = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &mainColorTexture);
    glTextureStorage2D(mainColorTexture, 1, GL_RGBA8, WIDTH, HEIGHT);

    glTextureParameteri(mainColorTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mainColorTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(mainColorTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mainColorTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create main depth texture
    mainDepthTexture = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &mainDepthTexture);
    glTextureStorage2D(mainDepthTexture, 1, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

    // Create and bind main FBO to main texture and depth texture
    mainFBO = 0;
    glCreateFramebuffers(1, &mainFBO);
    glNamedFramebufferTexture(mainFBO, GL_COLOR_ATTACHMENT0, mainColorTexture, 0);
    glNamedFramebufferTexture(mainFBO, GL_DEPTH_STENCIL_ATTACHMENT, mainDepthTexture, 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glNamedFramebufferDrawBuffers(mainFBO, 1, drawBuffers);

    if (glCheckNamedFramebufferStatus(mainFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "FBO not complete!" << std::endl;
}

void renderTextureViewport(Scene *scene) {
    ImGui::Begin("Texture Viewport");

    const float targetRatio = 16.0f / 9.0f;

    ImVec2 avail = ImGui::GetContentRegionAvail();

    float width  = avail.x;
    float height = width / targetRatio;

    if (height > avail.y)
    {
        height = avail.y;
        width  = height * targetRatio;
    }

    float offsetX = (avail.x - width) * 0.5f;
    float offsetY = (avail.y - height) * 0.5f;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

    ImGui::Image(
            (ImTextureID)(intptr_t)mainColorTexture,
            ImVec2(width, height),
            ImVec2(0, 1),
            ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 imageMin = ImGui::GetItemRectMin();

        // Position relative à l'image
        float localX = mousePos.x - imageMin.x;
        float localY = mousePos.y - imageMin.y;

        // Sécurité (si jamais resize en cours)
        localX = glm::clamp(localX, 0.0f, width);
        localY = glm::clamp(localY, 0.0f, height);

        // Génération du rayon
        glm::vec3 ray = scene->camera.getMouseRay(
                localX,
                localY,
                static_cast<int>(width),
                static_cast<int>(height)
        );

        bool hitSomething = false;

        for (auto mesh : scene->meshes)
        {
            if (!mesh->is_visible)
                continue;

            if (mesh->rayIntersection(scene->camera.position, ray))
            {
                mesh->selected = !mesh->selected;
                hitSomething = true;
                break;
            }
        }

        if (!hitSomething) {
            std::cout << "Void Raycast\n";
            for (auto mesh : scene->meshes) {
                mesh->selected = false;
                if (!mesh->is_visible)
                    continue;

                mesh->applyAndUpdate(scene->modifier);
            }
            scene->modifier.clear();
        }
    }

    ImGui::End();
}
}