#include <iostream>
#include "gui.hh"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"

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

void newFrame() {
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

//    const ImGuiViewport* main_vp = ImGui::GetMainViewport();
//    ImGui::SetNextWindowPos(main_vp->Pos);
//    ImGui::SetNextWindowSize(main_vp->Size);
//    ImGui::SetNextWindowViewport(main_vp->ID);
//
//    ImGui::Begin("MainDockSpace");
//    ImGuiID dockspace_id = ImGui::GetID("MyMainDockSpace");
//    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
//    ImGui::End();
//    if (!dock_built)
//    {
//        dock_built = true;
//        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//        ImGui::DockBuilderDockWindow("Viewport", dockspace_id);
//        ImGui::DockBuilderFinish(dockspace_id);
//    }

//        Gui::renderTextureViewport();
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

void setUpTextureViewport() {
    // Main texture
    glCreateTextures(GL_TEXTURE_2D, 1, &colorTex);
    glTextureStorage2D(colorTex, 1, GL_RGBA8, WIDTH, HEIGHT);
    glTextureParameteri(colorTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(colorTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(colorTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(colorTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Depth Renderbuffer
    glCreateRenderbuffers(1, &depthRbo);
    glNamedRenderbufferStorage(depthRbo, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

    // FBO
    glCreateFramebuffers(1, &fbo);
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colorTex, 0);
    glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

    // Verification
    if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer for main texture done." << std::endl;
    }
}

void renderTextureViewport() {
    ImGui::Begin("Viewport");

    ImVec2 avail = Gui::getResizeAvail();

    // OpenGL texture ID → ImGui
    ImGui::Image((void*)(intptr_t)Gui::colorTex,
                 avail,
                 ImVec2(0.0f, 1.0f),  // flip vertical
                 ImVec2(1.0f, 0.0f));

    ImGui::End();
}

ImVec2 getResizeAvail() {
    ImVec2 avail = ImGui::GetContentRegionAvail();
//    int newW = (int)avail.x;
//    int newH = (int)avail.y;
//
//    if (newW > 0 && newH > 0 &&
//        (newW != lastW || newH != lastH))
//    {
//        lastW = newW;
//        lastH = newH;
//
//        glTextureStorage2D(colorTex, 1, GL_RGBA8, newW, newH);
//        glNamedRenderbufferStorage(depthRbo, GL_DEPTH24_STENCIL8, newW, newH);
//    }
    return avail;
}
}