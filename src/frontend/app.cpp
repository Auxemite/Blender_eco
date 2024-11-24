#include "app.hh"

using namespace std;

App::App(){
    env = Env();
}

void App::Windows()
{
    if (env.scene.focus_mesh != nullptr)
        MeshOptions();

    TreeNode(env);
    Material();
    MainOptions();
    ShadersOptions();

    ImGui::Begin("End options");
    saveFile(env.scene.focus_mesh);
    ImGui::SameLine();
    raycastRender(env);

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    UI_inputs(env, io, pos);
    ImGui::End();

//    ImGui::ShowDemoWindow();
}

void App::ShadersOptions() {
    ImGui::Begin("Shaders Options");
    if (ImGui::Button("Base")) { render_mode = 0; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Normals")) { render_mode = 1; env.render_all(); }
    ImGui::SameLine();
    if (ImGui::Button("Phong")) { render_mode = 2; env.render_all(); }
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
    Color cc = env.scene.lights[0]->color;
    if (env.scene.focus_mesh != nullptr)
        cc = env.scene.focus_mesh->texture.material.color;
    static ImVec4 color = ImVec4(cc.r, cc.g, cc.b, 1.0f);
    colorSetup(color);

    if (env.scene.focus_mesh != nullptr) {
        ImGui::SliderFloat("kd", &env.scene.kd, 0, 1);
        ImGui::SliderFloat("ks", &env.scene.ks, 0, 1);
        ImGui::SliderFloat("ns", &env.scene.ns, 1, 150);

        if (ImGui::Button("Update Material")) {
            Texture texture = env.scene.focus_mesh->texture.material.texture;
            Color material_color = Color(color.x, color.y, color.z);
            env.scene.change_material(material_color, {texture.kd, texture.ks, texture.ns});
            env.render();
        }
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

void App::MeshOptions() {
    ImGui::Begin("Actions");
    ImGui::Text("Main Mesh Actions : ");
    move_mesh(env);

    if (env.scene.selected_mode != 3) {
        scale_mesh(env);
        rotate_mesh(env);
    }

    ImGui::Text("Special Mesh Actions : ");
    if (env.scene.editmode && env.scene.selected_mode == 1)
        extrude_mesh(env);
    ImGui::End();
}