#include "app.hh"

using namespace std;

App::App(){
    env = Env();
}

App::App(const char* filename) {
    env = Env(filename);
}

void App::Windows()
{
    ImGui::Begin("Viewport");
    ImGui::Text("pointer = %x", env.render_image);
    ImGui::SameLine();
    ImGui::Text("size = %d x %d", env.image.width, env.image.height);
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));
    ImGui::End();

    App::TreeNode();

    ImGui::Begin("Actions");

    if (ImGui::Button("Render")) { env.render(); }

    static float c1 = 1.00f;
    if (ImGui::Button("Move Camera X")) { env.move_camera(); }
    ImGui::SameLine();
    ImGui::DragFloat("Cam X", &c1, 0.005f);

    static float c2 = 1.00f;
    if (ImGui::Button("Move Camera Y")) { env.move_camera(); }
    ImGui::SameLine();
    ImGui::DragFloat("Cam Y", &c2, 0.005f);

    static float c3 = 1.00f;
    if (ImGui::Button("Move Camera Z")) { env.move_camera(); }
    ImGui::SameLine();
    ImGui::DragFloat("Cam Z", &c3, 0.005f);

    static float v1 = 1.00f;
    if (ImGui::Button("Move X")) { env.move_x(v1); }
    ImGui::SameLine();
    ImGui::DragFloat("X", &v1, 0.005f);

    static float v2 = 1.00f;
    if (ImGui::Button("Move Y")) { env.move_y(v2); }
    ImGui::SameLine();
    ImGui::DragFloat("Y", &v2, 0.005f);

    static float v3 = 1.00f;
    if (ImGui::Button("Move Z")) { env.move_z(v3); }
    ImGui::SameLine();
    ImGui::DragFloat("Z", &v3, 0.005f);

    static float radius = 1.00f;
    ImGui::DragFloat("Radius", &radius, 0.005f);
    if (ImGui::Button("Grow")) { env.grow(radius); }
    ImGui::SameLine();
    if (ImGui::Button("Shrink")) { env.shrink(radius); }


    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::TreeNode() {
    ImGui::Begin("Tree");
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    if (ImGui::TreeNode("Objects"))
    {
        static int selection_mask = (1 << 2);
        int node_clicked = -1;
        for (int i = 0; i < env.scene.objects.size(); i++)
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << i)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            string obj_type = typeid(*env.scene.objects[i]).name();
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "%d Object %s", i, obj_type.c_str());
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                node_clicked = i;
                // BEHAVIOR IS HERE
                env.change_focus(i, env.scene.objects[i]);
            }
        }
        if (node_clicked != -1)
            selection_mask = (1 << node_clicked);
        if (align_label_with_current_x_position)
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    PrintObjInfo();
    ImGui::End();
}

void App::PrintObjInfo() {
    string obj_type = env.focus_obj.obj_type;
    string text = "type : " + obj_type + "\n";
    if (obj_type == "Sphere")
        text += "Center : " + env.focus_obj.origin.to_string() + "\n"
                + "Radius : " + std::to_string(env.focus_obj.radius);
    if (obj_type == "Plane")
        text += "Origin : " + env.focus_obj.origin.to_string() + "\n"
                + "Normal : " + env.focus_obj.normal_.to_string() + "\n"
                + "Grille : " + std::to_string(env.focus_obj.grille);
    if (obj_type == "Triangle")
        text += "A : " + env.focus_obj.a.to_string() + "\n"
                "B : " + env.focus_obj.b.to_string() + "\n"
                "C : " + env.focus_obj.c.to_string() + "\n"
                + "Normal : " + env.focus_obj.normal_.to_string();
//    if (obj_type == "Mesh")
    ImGui::Text(text.c_str());
}