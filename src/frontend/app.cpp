#include <cstdlib>
#include "app.hh"

using namespace std;

App::App(){
    unsigned int size = WIDTH * HEIGHT * 3; // size = w * h * rgb
    char_data = (unsigned char *)calloc(size + 1, sizeof(unsigned char));
    for (int i = 0; i < size; ++i) {
        char_data[i] = 255;
    }
    char_data[size] = '\0';

    // Opengl code to bind image to a texture
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, char_data);

    glDeleteTextures(1, &render_image);
    render_image = image_texture;
}

void App::Windows()
{
    ImGui::Begin("Window 1");
    ImGui::End();

    ImGui::Begin("Window 3");
    ImGui::End();

    ImGui::Begin("Window 2");
    ImGui::Image((void*)(intptr_t)render_image,
                 ImVec2(static_cast<float>(WIDTH), static_cast<float>(HEIGHT)));
    ImGui::End();

    ImGui::ShowDemoWindow();
}

// To update image on screen
void App::UpdateTexture() {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, char_data);
}

void App::Inputs(const ImGuiIO& io, ImVec2 pos) {
//    float region_sz = 16.0f;
    float region_x = io.MousePos.x - pos.x;// - region_sz * 0.5f;
    float region_y = io.MousePos.y - pos.y;// - region_sz * 0.5f;
    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
    ImGui::SameLine();
    ImGui::Text("Mouse down:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        if (io.MouseDownDuration[i] > 0.1)
            return;
        if (region_x >= 0.0f && region_x <= 1280 && region_y >= 0.0f && region_y <= 720)
        if (ImGui::IsMouseDown(i)) {
            ImGui::SameLine();
            ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
            // MOUSE CLICKS HERE
        }
    }
    ImGui::SameLine();
    ImGui::Text("Keys down:");
    struct funcs {
        static bool IsLegacyNativeDupe(ImGuiKey key) {
            return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1;
        }
    };

    auto start_key = (ImGuiKey)0;
    for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
            continue;
        ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        // KEYBOARD CLICKS HERE
    }
}