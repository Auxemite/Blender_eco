#include <filesystem>
#include "app_utils.hh"
namespace fs = std::filesystem;

void saveFile(Mesh *focus_mesh) {
    if (ImGui::Button("Save file as"))
        ImGui::OpenPopup("save_file");
    if (ImGui::BeginPopup("save_file"))
    {
        static char filename[128] = "";
        ImGui::InputText("File Name", filename, IM_ARRAYSIZE(filename));
        ImGui::SameLine();
        if (ImGui::Button("Save file")) {
            if (focus_mesh == nullptr)
                std::cerr << "Save Mesh Error: NO MESH SELECTED\n";
            else
                focus_mesh->to_dot_obj(filename);
        }
        ImGui::EndPopup();
    }
}

void raycastRender(Env& env) {
    if (ImGui::Button("Raycast Render")) {
        env.create_texture();
        env.image.render(env.scene, bg, true, true);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, env.image.width, env.image.height,
                        GL_RGB, GL_UNSIGNED_BYTE, env.image.char_data);
        display_image = true;
    }
    if (display_image) {
        ImGui::Begin("Render");
        ImGui::Image((void*)(intptr_t)env.render_image,
                     ImVec2(static_cast<float>(env.image.width), static_cast<float>(env.image.height)));

        if (ImGui::Button("Change background"))
        {
            ImGui::OpenPopup("change_bg");
            std::string path = "../data/";
            mesh_names.clear();
            for (const auto & entry : fs::directory_iterator(path))
                if (entry.path().extension() == ".ppm")
                    mesh_names.push_back(entry.path().stem().string());
        }
        if (ImGui::BeginPopup("change_bg"))
        {
            ImGui::SeparatorText("Background Images");
            for (const std::string& filename : mesh_names) {
                std::string ui_filename = filename;
                ui_filename[0] = toupper(ui_filename[0]);
                if (ImGui::Selectable(ui_filename.c_str())) {
                    env.change_bg(filename);
                }
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
                env.image.save_as_ppm("../test/" + std::string(filename) + ".ppm");
                display_image = false;
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            display_image = false;
        ImGui::End();
    }
}

void colorSetup(ImVec4& color) {
    static bool alpha_preview = true;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = true;
    static bool options_menu = true;
    static bool hdr = false;
    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

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
}

void addMesh(Env& env) {
    if (ImGui::Button("Add Mesh"))
    {
        ImGui::OpenPopup("add_mesh");
        std::string path = "../data/";
        mesh_names.clear();
        for (const auto & entry : fs::directory_iterator(path))
            if (entry.path().extension() == ".obj")
                mesh_names.push_back(entry.path().stem().string());
    }
    if (ImGui::BeginPopup("add_mesh"))
    {
        ImGui::SeparatorText("Mesh Types");
        for (const std::string& filename : mesh_names) {
            std::string ui_filename = filename;
            ui_filename[0] = toupper(ui_filename[0]);
            if (ImGui::Selectable(ui_filename.c_str())) {
                env.add_mesh(filename);
            }
        }
        ImGui::EndPopup();
    }
}