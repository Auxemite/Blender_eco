#include "app.hh"

using namespace std;

void App::Windows()
{
    ImGui::Begin("Viewport");
    ImGui::Text("pointer = %x", env.render_image);
    ImGui::Text("size = %d x %d", env.image.width, env.image.height);
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));
    ImGui::End();

//    ImGui::ShowDemoWindow();

    ImGui::Begin("Actions");

    if (ImGui::Button("Render")) { env.render(); }
    if (ImGui::Button("Move Camera")) { env.move_camera(); }
    if (ImGui::Button("Select Sphere")) { env.change_focus(); }
    ImGui::SameLine();
    ImGui::Text("Selected Sphere : %d", env.focus_index);

    if (ImGui::Button("Move Right")) { env.move_right(); }
    ImGui::SameLine();
    if (ImGui::Button("Move Left")) { env.move_left(); }

    if (ImGui::Button("Grow")) { env.grow(); }
    ImGui::SameLine();
    if (ImGui::Button("Shrink")) { env.shrink(); }

    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::End();
}

App::App()
{
    env = Env();
}

App::App(const char* filename)
{
    env = Env(filename);
}