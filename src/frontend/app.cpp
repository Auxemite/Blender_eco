//
// Created by ernes on 02/04/2024.
//

#include "app.h"

using namespace std;

inline double aspect_ratio = 16.0 / 9.0;
inline int default_width = 640;
inline int default_height = static_cast<int>(default_width / aspect_ratio);

void App::Windows()
{
    ImGui::Begin("Viewport");
    ImGui::Text("pointer = %x", render_image);
    ImGui::Text("size = %d x %d", image.width, image.height);
    ImGui::Image((void*)(intptr_t)render_image,
                 ImVec2(static_cast<float>(image.width), static_cast<float>(image.height)));
    ImGui::End();

    ImGui::ShowDemoWindow();

    ImGui::Begin("Actions");
    if (ImGui::Button("Render")) {
        image.render(scene);
        viewport_update();
    }

    if (ImGui::Button("Move Camera")) {
        Point3 p = scene.camera.center;
        double angle = PI / 8;
        scene.camera.update_cam(Point3(cos(angle) * p.x - sin(angle) * p.z, 0, sin(angle) * p.x + cos(angle) * p.z) + Point3(0, 5, 0));

        image.render(scene);
        viewport_update();
    }

    if (ImGui::Button("Move Sphere 1 Right")) {
        scene.spheres[1].center.x += 0.5;
        image.render(scene);
        viewport_update();
    }

    ImGui::SameLine();

    if (ImGui::Button("Move Sphere 1 Left")) {
        scene.spheres[1].center.x -= 0.5;
        image.render(scene);
        viewport_update();
    }

    if (ImGui::Button("Grow Sphere 1")) {
        scene.spheres[1].radius += 0.2;
        image.render(scene);
        viewport_update();
    }

    ImGui::SameLine();

    if (ImGui::Button("Shrink Sphere 1")) {
        scene.spheres[1].radius -= 0.2;
        image.render(scene);
        viewport_update();
    }

    ImGui::End();
}

unsigned char* App::convertDataToGLRGB(const vector<vector<Color>>& data, int width, int height)
{
    unsigned char* char_data;
    size_t size = width * height * 3;
    char_data = (unsigned char *)calloc(size + 1, sizeof(unsigned char));

    int k = 0;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            char_data[k++] = static_cast<unsigned char>(data[i][j].r * 255);
            char_data[k++] = static_cast<unsigned char>(data[i][j].g * 255);
            char_data[k++] = static_cast<unsigned char>(data[i][j].b * 255);
        }
    }
    char_data[size] = '\0';
    return char_data;
}

void App::viewport_update()
{
    unsigned char* image_data = convertDataToGLRGB(image.data, image.width, image.height);

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    glDeleteTextures(1, &render_image);
    render_image = image_texture;
}

App::App()
{
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    unsigned char* image_data = convertDataToGLRGB(image.data, image.width, image.height);

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    render_image = image_texture;
}

App::App(const char* filename)
{
    image = load_image(filename);
    scene = Scene(image.width, image.height);
    unsigned char* image_data = convertDataToGLRGB(image.data, image.width, image.height);

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    render_image = image_texture;
}

void App::RenderUI()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", nullptr, &opt_fullscreen);
            ImGui::MenuItem("Padding", nullptr, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    Windows();

    ImGui::End();
}
