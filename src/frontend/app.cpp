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

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));

    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::SameLine();
    if (ImGui::Button("Add Mesh")) { env.add_mesh("cube"); }

    ImGui::SameLine();
    if (ImGui::Button("Delete Mesh")) { env.delete_mesh(env.focus_mesh); }

    ImGui::SameLine();
    SelectMesh(io, pos);

    ImGui::End();


    App::TreeNode();

    ImGui::Begin("Actions");

    if (env.focus_mesh != nullptr) {
        MeshOptions();
    }

    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::SelectMesh(const ImGuiIO& io, ImVec2 pos) {
    float region_sz = 32.0f;
    float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
    float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
    if (region_x < 0.0f) { region_x = 0.0f; }
    else if (region_x > 1280 - region_sz) { region_x = 1280 - region_sz; }
    if (region_y < 0.0f) { region_y = 0.0f; }
    else if (region_y > 720 - region_sz) { region_y = 720 - region_sz; }
    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
    ImGui::SameLine();
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
        if (key == 513) { // Left Arrow
            env.move_camera_y(-15);
            env.render();
        }
        else if (key == 514) { // Right Arrow
            env.move_camera_y(15);
            env.render();
        }
        else if (key == 515) { // Up Arrow
            env.scene.camera.update_cam(env.scene.camera.center + Point3(0,1,0));
            env.render();
        }
        else if (key == 516) { // Down Arrow
            env.scene.camera.update_cam(env.scene.camera.center - Point3(0,1,0));
            env.render();
        }
        else if (key == 655) { // Left Click
            env.select_mesh(region_x, region_y);
        }
    }
}

void App::TreeNode() {
    ImGui::Begin("Tree");
    for (int i = 0; i < env.scene.meshes.size(); i++) {
        std::string name = "> Mesh " + to_string(i);
        if (ImGui::Button(name.c_str()))
            env.change_focus(env.scene.meshes[i]);
        ImGui::SameLine();
        TreeMesh(env.scene.meshes[i], i);
    }

    if (env.focus_mesh != nullptr) {
        PrintObjInfo();
    }

    ImGui::End();
}

void App::TreeMesh(Mesh *mesh, int index) {
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    std::string name = "Faces Mesh " + to_string(index);
    if (ImGui::TreeNode(name.c_str()))
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
}

void App::PrintObjInfo() {
    if (env.focus_mesh == nullptr) {
        ImGui::Text("No Mesh Selected");
        return;
    }
    string text = "type : Mesh\n";
    text += "Number of Faces : " + to_string(env.focus_mesh->faces.size()) + "\n";
    text += "Number of Edges : " + to_string(env.focus_mesh->points.size()) + "\n";
    text += "Edges :\n";
    for (auto & edge : env.focus_mesh->points) {
        text += edge->to_string() + "\n";
    }
    ImGui::Text(text.c_str());
}