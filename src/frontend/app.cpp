#include "app.hh"

using namespace std;

App::App()
{
    env = Env();
}

App::App(const char* filename)
{
    env = Env(filename);
}

void App::Windows()
{
    ImGui::Begin("Viewport");
    ImGui::Text("pointer = %x", env.render_image);
    ImGui::Text("size = %d x %d", env.image.width, env.image.height);
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));
    ImGui::End();

//    ImGui::ShowDemoWindow();
    App::TreeNode();

    ImGui::Begin("Actions");

    if (ImGui::Button("Render")) { env.render(); }
    if (ImGui::Button("Move Camera")) { env.move_camera(); }

    if (ImGui::Button("Move Right")) { env.move_right(); }
    ImGui::SameLine();
    if (ImGui::Button("Move Left")) { env.move_left(); }

    if (ImGui::Button("Grow")) { env.grow(); }
    ImGui::SameLine();
    if (ImGui::Button("Shrink")) { env.shrink(); }

    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::End();
}

void App::TreeNode() {
    ImGui::Begin("Tree");
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    if (ImGui::TreeNode("Objects"))
    {
        static int selection_mask = (1 << 2);
        int node_clicked = -1;
        for (int i = 0; i < env.scene.spheres.size(); i++)
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << i)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Object %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                node_clicked = i;
                // BEHAVIOR IS HERE
                env.change_focus(i, "sphere");
            }
        }
        if (node_clicked != -1)
            selection_mask = (1 << node_clicked);
        if (align_label_with_current_x_position)
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    ImGui::End();
}