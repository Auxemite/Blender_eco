#include "app.hh"

using namespace std;

const char* names[] = { "Cube", "Plane", "Triangle", "Cone", "Sphere", "Icosphere", "Cylinder", "Donut", "Monkey"};
const char* name_bgs[] = { "Sky", "Sunset", "Forest", "Synthwave", "Space", "Mars", "Earth"};

App::App(){
    env = Env();
}

void App::Windows()
{
    if (env.scene.focus_mesh != nullptr) {
        MeshOptions();
        Material();
    }

//    CameraOption();

    App::TreeNode();

    App::MainOptions();

    ImGui::Begin("Shaders Options");
    if (ImGui::Button("Base")) { render_mode = 0; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Normals")) { render_mode = 1; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Phong")) { render_mode = 2; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Ambient")) { render_mode = 3; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Diffuse")) { render_mode = 4; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Specular")) { render_mode = 5; env.render_all(); }
    ImGui::End();

    ImGui::Begin("End options");
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
    if (ImGui::Button("Render")) {
        env.create_texture();
        env.image.render(env.scene, bg, true, true);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, env.image.width, env.image.height,
                        GL_RGB, GL_UNSIGNED_BYTE, env.image.char_data);
        display_image = true;
    }
    if (display_image)
        Rendering();

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    App::Inputs(io, pos);

    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::Rendering() {
    ImGui::Begin("Render");
    ImGui::Image((void*)(intptr_t)env.render_image,
                 ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));

    if (ImGui::Button("Change background"))
        ImGui::OpenPopup("change_bg");
    if (ImGui::BeginPopup("change_bg"))
    {
        ImGui::SeparatorText("Background Images");
        for (auto & i : name_bgs)
            if (ImGui::Selectable(i)) {
                std::string name = i;
                name[0] = tolower(name[0]);
                env.change_bg(name);
            }
        ImGui::EndPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Render"))
        ImGui::OpenPopup("save_render");
    if (ImGui::BeginPopup("save_render"))
    {
        static char filename[128] = "";
        ImGui::InputText("File Name", filename, IM_ARRAYSIZE(filename));
        ImGui::SameLine();
        if (ImGui::Button("Save file")) {
            env.image.save_as_ppm("../test/" + string(filename) + ".ppm");
            display_image = false;
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
        display_image = false;
    ImGui::End();
}

void App::MainOptions() {
    ImGui::Begin("Main Options");
    ImGui::Text("Camera Settings");
    ImGui::SliderInt("Zoom Speed", &speed_zoom, 5, 20);
    ImGui::SliderInt("Rotation Speed", &speed_rotation, 10, 50);
    ImGui::SliderFloat("Mouse Sensibility", &sensitivity, 0.1, 2);
    ImGui::SliderFloat("Zoom Sensibility", &zoom_sensitivity, 0.1, 2);

    if (!env.scene.editmode) {
        if (ImGui::Button("Edit Mode"))
            env.edit_mode();
    }
    else {
        if (ImGui::Button("Normal Mode"))
            env.normal_mode();

        ImGui::Text("Selection Mode : ");
        ImGui::SameLine();
        if (ImGui::RadioButton("Mesh", &env.scene.selected_mode, 0)) { env.scene.update_selection_mode(); env.render(); }
        ImGui::SameLine();
        if (ImGui::RadioButton("Face", &env.scene.selected_mode, 1)) { env.scene.update_selection_mode(); env.render(); }
//        ImGui::SameLine();
//        if (ImGui::RadioButton("Edge", &env.scene.selected_mode, 2)) { env.scene.update_selection_mode(); env.render(); }
        ImGui::SameLine();
        if (ImGui::RadioButton("Summit", &env.scene.selected_mode, 3)) { env.scene.update_selection_mode(); env.render(); }
    }
//    if (ImGui::RadioButton("Fast Selection", &env.fast_selection, 1)) { env.render(); }
//    ImGui::SameLine();
//    if (ImGui::RadioButton("Classic Selection", &env.fast_selection, 0)) { env.render(); }

    ImGui::Checkbox("Grid", &env.scene.activate_grid);
    ImGui::Checkbox("Alpha Features", &alpha_feature);
    if (ImGui::Button("Update"))
        env.render();
    ImGui::SameLine();
    if (ImGui::Button("Update All"))
        env.render_all();
    ImGui::End();
}

void App::Material() {
    ImGui::Begin("Material");
    static bool alpha_preview = true;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = true;
    static bool options_menu = true;
    static bool hdr = false;
    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    if (env.scene.focus_mesh == nullptr)
        return;

    Color cc = env.scene.focus_mesh->texture.material.color;
    static ImVec4 color = ImVec4(cc.r, cc.g, cc.b, 1.0f);
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

    ImGui::SliderFloat("kd", &env.scene.kd, 0, 1);
    ImGui::SliderFloat("ks", &env.scene.ks, 0, 1);
    ImGui::SliderFloat("ns", &env.scene.ns, 1, 150);

    if (ImGui::Button("Update Material")) {
        Texture texture = env.scene.focus_mesh->texture.material.texture;
        Color material_color = Color(color.x, color.y, color.z);
        env.scene.change_material(material_color, {texture.kd, texture.ks, texture.ns});
        env.render();
    }

    ImGui::Text("Light Options");
    ImGui::SliderFloat("power", &env.scene.lights[0]->power, 1, 150);
    ImGui::SliderFloat("Light X", &env.scene.lights[0]->center.x, -5, 5);
    ImGui::SliderFloat("Light Y", &env.scene.lights[0]->center.y, -5, 5);
    ImGui::SliderFloat("Light Z", &env.scene.lights[0]->center.z, -5, 5);

    if (ImGui::Button("Update Light Color"))
        env.scene.lights[0]->color = Color(color.x, color.y, color.z);
    ImGui::End();
}

void App::CameraOption() {
    ImGui::Begin("Camera");

    static int angle[3] = { 0, 15, 0};
    static int move[3] = { 0, 0, 0};
    ImGui::SliderInt3("Angle", angle, -45, 45);
    if (ImGui::Button("Rotate Camera")) {
        env.scene.move_camera_x(angle[0]);
        env.scene.move_camera_y(angle[1]);
        env.scene.move_camera_z(angle[2]);
        env.render();
    }

    ImGui::SliderInt3("X,Y,Z", move, -10, 10);
    if (ImGui::Button("Move Camera")) {
        env.scene.camera.update_cam(Point3(move[0], move[1], move[2]));
        env.render();
    }

    ImGui::End();
}

void App::TreeNode() {
    ImGui::Begin("Tree");
    if (ImGui::Button("Add Mesh"))
        ImGui::OpenPopup("add_mesh");
    if (ImGui::BeginPopup("add_mesh"))
    {
        ImGui::SeparatorText("Mesh Types");
        for (auto & i : names)
            if (ImGui::Selectable(i)) {
                std::string name = i;
                name[0] = tolower(name[0]);
                env.add_mesh(name);
            }
        ImGui::EndPopup();
    }
    if (env.scene.focus_mesh != nullptr) {
        ImGui::SameLine();
        if (ImGui::Button("Delete Mesh")) { env.delete_mesh(); }
    }
    for (int i = 0; i < env.scene.meshes.size(); i++) {
        std::string name = "> Mesh " + to_string(i);
        if (ImGui::Button(name.c_str())) {
            env.scene.change_focus(env.scene.meshes[i], i);
            env.render_all();
        }
        ImGui::SameLine();
        if (!env.scene.meshes[i]->watch) {
            ImGui::PushID(i);
            if (ImGui::Button("<Ø>")) {
                env.scene.meshes[i]->watch = true;
                env.render();
            }
            ImGui::PopID();
        }
        else {
            ImGui::PushID(i);
            if (ImGui::Button("<O>")) {
                env.scene.meshes[i]->watch = false;
                env.render();
            }
            ImGui::PopID();
        }
        ImGui::SameLine();
        TreeMesh(env.scene.meshes[i], i);
    }

    if (env.scene.focus_mesh != nullptr) {
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
                env.scene.change_focus(mesh, mesh->faces[i]);
                env.render();
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
    ImGui::Begin("Actions");
    ImGui::Text("Main Mesh Actions : ");
    if (env.scene.editmode) {
        static float v1 = 1.00f;
        if (ImGui::Button("Move X")) {
            env.scene.move_x(v1);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("X", &v1, -5, 5);

        static float v2 = 1.00f;
        if (ImGui::Button("Move Y")) {
            env.scene.move_y(v2);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Y", &v2, -5, 5);

        static float v3 = 1.00f;
        if (ImGui::Button("Move Z")) {
            env.scene.move_z(v3);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Z", &v3, -5, 5);
    }
    else {
        ImGui::SliderFloat("X", &env.scene.dec_x, -5, 5);
        ImGui::SliderFloat("Y", &env.scene.dec_y, -5, 5);
        ImGui::SliderFloat("Z", &env.scene.dec_z, -5, 5);
    }

    if (env.scene.selected_mode != 3) {
        static float scale = 1.00f;
        if (ImGui::Button("Scale")) {
            env.scene.scale(scale);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Value", &scale, 0.1f, 3);

        static float a1 = 0;
        ImGui::SliderAngle("RX", &a1, -90, 90);
        static float a2 = 0;
        ImGui::SliderAngle("RY", &a2, -90, 90);
        static float a3 = 0;
        ImGui::SliderAngle("RZ", &a3, -90, 90);
        if (ImGui::Button("Rotate Y")) {
            env.scene.rotate_xyz(a1, a2, a3);
            env.render();
        }
    }

    ImGui::Text("Special Mesh Actions : ");
    if (env.scene.editmode && env.scene.selected_mode == 1) {
        static float move[3] = { 0, 0, 0};
        ImGui::SliderFloat3("X Y Z", move, -2, 2);
        if (ImGui::Button("Extrude")) {
            env.scene.extrude(move[0], move[1], move[2]);
            env.render();
        }
        static float thickness = 0;
        ImGui::SliderFloat("Thickness", &thickness, -2, 2);
        if (ImGui::Button("Extrude Along Normal")) {
            env.scene.extrude_along_normal(thickness);
            env.render();
        }
        if (ImGui::Button("Extrude Along Point")) {
            env.scene.extrude_along_points_normalized(thickness);
            env.render();
        }
    }
    ImGui::End();
}

void App::PrintObjInfo() const {
    if (env.scene.focus_mesh == nullptr) {
        ImGui::Text("No Mesh Selected");
        return;
    }
    string text = "type : Mesh\n";
    text += "Number of Faces : " + to_string(env.scene.focus_mesh->faces.size()) + "\n";
    text += "Number of Summit : " + to_string(env.scene.focus_mesh->points.size()) + "\n";
    text += "Summits :\n";
    for (auto & summit : env.scene.focus_mesh->points) {
        text += summit->to_string() + "\n";
    }
//    text += "Faces :\n";
//    for (auto & face : env.scene.focus_mesh->faces) {
//        text += face->a->to_string() + " ";
//        text += face->b->to_string() + " ";
//        text += face->c->to_string() + "\n";
//    }
    ImGui::Text("%s", text.c_str());
}

void App::Inputs(const ImGuiIO& io, ImVec2 pos) {
//    float region_sz = 16.0f;
    float region_x = io.MousePos.x;// - pos.x;// - region_sz * 0.5f;
    float region_y = io.MousePos.y;// - pos.y;// - region_sz * 0.5f;
    if (region_x < 0.0f) { return; }
    else if (region_x > WIDTH) { return; }
    if (region_y < 0.0f) { return; }
    else if (region_y > HEIGHT) { return; }
    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
    ImGui::Text("Mouse down:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        if (io.MouseDownDuration[i] > 0.001)
            return;
        if (region_x >= 0.0f && region_x <= WIDTH && region_y >= 0.0f && region_y <= HEIGHT) {
            if (ImGui::IsMouseDown(i)) {
                ImGui::SameLine();
                ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
                env.update_camera();
                if (env.scene.selected_mode == 3) {
                    env.scene.select_summit(region_x, region_y);
                    env.render();
                }
                else {
                    env.scene.select_mesh(region_x, region_y);
                    env.render_all();
                }
            }
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
//        if (key == 513) { // Left Arrow
//            env.scene.move_camera_y(-15);
//            env.render();
//        }
//        else if (key == 514) { // Right Arrow
//            env.scene.move_camera_y(15);
//            env.render();
//        }
//        else if (key == 515) { // Up Arrow
//            env.scene.camera.update_cam(env.scene.camera.center + Point3(0,1,0));
//            env.render();
//        }
//        else if (key == 516) { // Down Arrow
//            env.scene.camera.update_cam(env.scene.camera.center - Point3(0,1,0));
//            env.render();
//        }
//        else if (key == 569) { // X
//            env.scene.delete_mesh();
//            env.render();
//        }
//        else if (key == 546) { // A
//            ImGui::OpenPopup("add_mesh");
//        }
//        else if (key == 571) { // Z
//            env.scene.zoom_camera(0.9);
//            env.render();
//        }
//        else if (key == 549) { // D
//            env.scene.zoom_camera(1.1);
//            env.render();
//        }
    }
}