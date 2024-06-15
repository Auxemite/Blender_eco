#include "app.hh"

using namespace std;

App::App(){
    env = Env();
}

App::App(const char* filename) {
    env = Env(filename);
}

void App::MainOptions() {
    ImGui::SameLine();
    if (env.photorealist) {
        if (ImGui::Button("Desactivate Render")) {
            env.photorealist = false;
            env.render();
        }
        ImGui::SameLine();

        const char* names[] = { "Sky", "Sky2", "Sunset", "Sunset2", "Synthwave", "Space", "Tree"};
        if (ImGui::Button("Change background"))
            ImGui::OpenPopup("change_bg");
        if (ImGui::BeginPopup("change_bg"))
        {
            ImGui::SeparatorText("Background Images");
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i])) {
                    std::string name = names[i];
                    name[0] = tolower(name[0]);
                    env.change_bg(name);
                }
            ImGui::EndPopup();
        }
    }
    else {
        if (ImGui::Button("Activate Render")) {
            env.photorealist = true;
            env.render();
        }
    }
    ImGui::SameLine();
    if (!env.editmode) {
        if (ImGui::Button("Edit Mode")) {
            env.editmode = true;
            env.render();
        }
    }
    else {
        if (ImGui::Button("Normal Mode")) {
            env.render();
            env.editmode = false;
        }
        ImGui::SameLine();

        ImGui::Text("| Selection Mode : ");
        ImGui::SameLine();
        if (ImGui::RadioButton("Mesh", &env.selected_mode, 0)) { env.update_selection_mode(); };
        ImGui::SameLine();
        if (ImGui::RadioButton("Face", &env.selected_mode, 1)) { env.update_selection_mode(); };
//        ImGui::SameLine();
//        if (ImGui::RadioButton("Edge", &env.selected_mode, 2)) {};
    }
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::RadioButton("Fast Selection", &env.fast_selection, 1); ImGui::SameLine();
    ImGui::RadioButton("Normal Selection", &env.fast_selection, 0);
}

void App::Windows()
{
    ImGui::Begin("Actions");
    if (env.focus_mesh != nullptr)
        MeshOptions();
    ImGui::End();

    CameraOption();

    ImGui::Begin("Material");
    if (env.focus_mesh != nullptr)
        Material();
    ImGui::End();

    App::TreeNode();

    ImGui::Begin("Viewport");

    MainOptions();

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));

    if (ImGui::Button("Save Render")) { env.image.save_as_ppm("../test/result.ppm"); }
//    if (ImGui::Button("Save File")) { env.image.save_as_ppm("../test/result.ppm"); }

    ImGui::SameLine();

    if (ImGui::Button("Save file as"))
        ImGui::OpenPopup("save_file");
    if (ImGui::BeginPopup("save_file"))
    {
        static char filename[128] = "";
        ImGui::InputText("File Name", filename, IM_ARRAYSIZE(filename));
        ImGui::SameLine();
        if (ImGui::Button("Save file"))
            env.save_mesh("../test/" + string(filename) + ".obj");
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    const char* names[] = { "Cube", "Plane", "Cone", "Sphere", "Icosphere", "Cylinder", "Donut", "Monkey"};

    if (ImGui::Button("Add Mesh"))
        ImGui::OpenPopup("add_mesh");
    if (ImGui::BeginPopup("add_mesh"))
    {
        ImGui::SeparatorText("Mesh Types");
        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
            if (ImGui::Selectable(names[i])) {
                std::string name = names[i];
                name[0] = tolower(name[0]);
                env.add_mesh(name);
            }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete Mesh")) { env.delete_mesh(env.focus_mesh); }


    ImGui::SameLine();
    SelectMesh(io, pos);

    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::Material() {
    static bool alpha_preview = true;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = true;
    static bool options_menu = true;
    static bool hdr = false;
    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    Color cc = env.focus_mesh->texture.mat.color;
    Texture texture1 = env.focus_mesh->texture.mat.texture;
    static ImVec4 color = ImVec4(cc.r, cc.g, cc.b, 255.0f / 255.0f);
    static float kd = texture1.kd;
    static float ks = texture1.ks;
    static float ns = texture1.ns;
    static bool side_preview = true;
    static int display_mode = 0;
    static int picker_mode = 0;
    ImGui::Checkbox("With Side Preview", &side_preview);

    ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
    ImGuiColorEditFlags flags = misc_flags;
    if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
    if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
    if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
    if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;
    if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;
    if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
    if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
    ImGui::ColorPicker4("MyColor##4", (float*)&color, flags);

    ImGui::SliderFloat("kd", &kd, 0, 1);
    ImGui::SliderFloat("ks", &ks, 0, 1);
    ImGui::SliderFloat("ns", &ns, 1, 150);

    if (ImGui::Button("Update Material")) {
        Texture texture = {kd, ks, ns};
        Color material_color = Color(color.x, color.y, color.z);
        env.change_material(material_color, texture);
    }
}

void App::CameraOption() {
    ImGui::Begin("Camera");

    static int angle[3] = { 0, 15, 0};
    static int move[3] = { 0, 0, 0};
    ImGui::SliderInt3("Angle", angle, -90, 90);
    if (ImGui::Button("Rotate Camera")) {
        env.move_camera_x(angle[0]);
        env.move_camera_y(angle[1]);
        env.move_camera_z(angle[2]);
        env.render();
    }

    ImGui::SliderInt3("X,Y,Z", move, -5, 5);
    if (ImGui::Button("Move Camera")) {
        env.scene.camera.update_cam(Point3(move[0], move[1], move[2]));
        env.render();
    }


    ImGui::End();
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
                env.change_focus(mesh, mesh->faces[i]);
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
    ImGui::Text("Main Mesh Actions : ");
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
    ImGui::SliderAngle("RX", &a1, -90, 90);

    static float a2 = 0;
    if (ImGui::Button("Rotate Y")) { env.rotate_y(a2); }
    ImGui::SameLine();
    ImGui::SliderAngle("RY", &a2, -90, 90);

    static float a3 = 0;
    if (ImGui::Button("Rotate Z")) { env.rotate_z(a3); }
    ImGui::SameLine();
    ImGui::SliderAngle("RZ", &a3, -90, 90);

    ImGui::Text("Special Mesh Actions : ");
    if (env.editmode) {
        static int move[3] = { 0, 0, 0};
        ImGui::SliderInt3("X Y Z", move, -3, 3);
        if (ImGui::Button("Extrude"))
            env.extrude(move[0], move[1], move[2]);
    }
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

