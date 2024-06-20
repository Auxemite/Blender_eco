#include "app.hh"

using namespace std;

App::App(){
}

void App::Windows()
{
    ImGui::Begin("Window 1");
    ImGui::End();

    ImGui::Begin("Window 3");
    ImGui::End();

    ImGui::Begin("Window 2");
    ImGui::End();

    ImGui::ShowDemoWindow();
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