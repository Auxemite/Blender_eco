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

    static int vec3i[4] = { 0, 15, 0};
    if (ImGui::Button("Move Camera")) {
        env.move_camera_x(vec3i[0]);
        env.move_camera_y(vec3i[1]);
        env.move_camera_z(vec3i[2]);
        env.render();
    }

    ImGui::SameLine();
    ImGui::SliderInt3("Angle on X,Y,Z", vec3i, 0, 90);

    ImGui::SameLine();
    if (env.photorealist) {
        if (ImGui::Button("Desactivate Render")) {
            env.photorealist = false;
            env.render();
        }
    }
    else {
        if (ImGui::Button("Activate Render")) {
            env.photorealist = true;
            env.render();
        }
    }

    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));

    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::End();

    App::TreeNode();

    ImGui::Begin("Actions");

    if (env.focus_mesh != nullptr) {
        MeshOptions();
    }

    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::TreeNode() {
    ImGui::Begin("Tree");
    for (int i = 0; i < env.scene.meshes.size(); i++) {
        string name = "> Mesh " + i;
        if (ImGui::Button(name.c_str()))
            env.change_focus(i, env.scene.meshes[i]);
        ImGui::SameLine();
        TreeMesh(env.scene.meshes[i]);
    }

    if (env.focus_mesh != nullptr) {
        PrintObjInfo();
    }

    ImGui::End();
}

void App::TreeMesh(Mesh *mesh) {
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    if (ImGui::TreeNode("Meshes"))
    {
        static int selection_mask = (1 << 2);
        int node_clicked = -1;
        for (int i = 0; i < mesh->faces.size(); i++)
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << i)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "> %s %d", "Face", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                node_clicked = i;
                // BEHAVIOR IS HERE
            }
        }
        if (node_clicked != -1)
            selection_mask = (1 << node_clicked);
        if (align_label_with_current_x_position)
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}

void App::MeshOptions() {
    static float v1 = 1.00f;
    if (ImGui::Button("Move X")) { env.move_x(v1); }
    ImGui::SameLine();
    ImGui::SliderFloat("X", &v1, -5, 5);

    static float v2 = 1.00f;
    if (ImGui::Button("Move Y")) { env.move_y(v2); }
    ImGui::SameLine();
    ImGui::SliderFloat("Y", &v2, -5, 5);

    static float v3 = 1.00f;
    if (ImGui::Button("Move Z")) { env.move_z(v3); }
    ImGui::SameLine();
    ImGui::SliderFloat("Z", &v3, -5, 5);

    static float scale = 1.00f;
    if (ImGui::Button("Scale")) { env.scale(scale); }
    ImGui::SameLine();
    ImGui::SliderFloat("Value", &scale, 0.1f, 3);

    static float a1 = 0;
    if (ImGui::Button("Rotate X")) { env.rotate_x(a1); }
    ImGui::SameLine();
    ImGui::SliderAngle("RX", &a1, 0, 180);

    static float a2 = 0;
    if (ImGui::Button("Rotate Y")) { env.rotate_y(a2); }
    ImGui::SameLine();
    ImGui::SliderAngle("RY", &a2, 0, 180);

    static float a3 = 0;
    if (ImGui::Button("Rotate Z")) { env.rotate_z(a3); }
    ImGui::SameLine();
    ImGui::SliderAngle("RZ", &a3, 0, 180);
    ImGui::Text("Keys down:");
    struct funcs {
        static bool IsLegacyNativeDupe(ImGuiKey key) {
            return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1;
        }
    };
    ImGuiKey start_key = (ImGuiKey)0;
    for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
            continue;
        ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        if (key == 513) {
            env.move_camera_y(-15);
            env.render();
        }
        else if (key == 514) {
            env.move_camera_y(15);
            env.render();
        }
        else if (key == 515) {
            env.move_camera_x(10);
            env.move_camera_z(10);
            env.render();
        }
        else if (key == 516) {
            env.move_camera_x(-10);
            env.move_camera_z(-10);
            env.render();
        }
    }
}

void App::PrintObjInfo() {
    string text = "type : Mesh\n";
    text += "Number of Faces : " + to_string(env.focus_mesh->faces.size()) + "\n";
    text += "Number of Edges : " + to_string(env.focus_mesh->points.size()) + "\n";
//    text += "First edge : " + env.focus_mesh->points[0]->to_string();
    text += "Edges :\n";
    for (auto & edge : env.focus_mesh->points) {
        text += edge->to_string() + "\n";
    }
    ImGui::Text(text.c_str());
}