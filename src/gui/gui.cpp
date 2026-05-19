#include <string>
#include <filesystem>
#include "gui.hh"
#include "scene/scene.hh"
#include "shader/shaderUtils.hh"

namespace fs = std::filesystem;

namespace Gui {
    void mainGui(Scene& scene, EditMode::EditModeScene& editModeScene, VisualGrid& grid) {
//        ImGui::ShowDemoWindow();
        materials(scene);
        textures(scene);
        lights(scene);

        ImGui::Begin("Mode");
        ImGui::Checkbox("Grid##visualGridToggle", &grid.activateGrid_);
        ImGui::SameLine();
        ImGui::Text(" | ");
        ImGui::SameLine();
        editModeScene.editModeModulator(scene);

        ImGui::SameLine();
        if (ImGui::Button("Basic##shaderBasic")) {
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/basic");
            Env::textureEnabled = false;
            Env::PBREnabled = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Wireframe##shaderWireframe")) {
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
            Env::textureEnabled = false;
            Env::PBREnabled = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Texture##shaderTexture")) {
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/texture");
            Env::textureEnabled = true;
            Env::PBREnabled = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("PBR##shaderPBR")) {
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/pbr");
            Env::textureEnabled = true;
            Env::PBREnabled = true;
        }
        ImGui::End();

        ImGui::Begin("Action");
        if (Env::editmode)
            editModeScene.modifierModulator();
        else
            scene.modifierModulator();

        ImGui::End();

        meshTreeNode(scene);
    }

    void meshTreeNode(Scene& scene) {
        ImGui::Begin("Tree");
        addMesh(scene);
        ImGui::SameLine();
        scene.duplicateMeshModulator();
        ImGui::SameLine();
        scene.deleteMeshModulator();
        scene.treeMeshModulator();

        ImGui::End();
    }

    void addMesh(Scene& scene) {
        if (ImGui::Button("New##addNewMesh"))
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
                    scene.addMesh("../data/" + filename + ".obj");
                }
            }
            ImGui::EndPopup();
        }
    }

    void materials(Scene& scene) {
        ImGui::Begin("Materials");
        scene.materialModulator();
        ImGui::End();
    }

    void textures(Scene& scene) {
        ImGui::Begin("Textures");
        scene.textureModulator();
        ImGui::End();
    }

    void lights(Scene& scene) {
        ImGui::Begin("Lights");
        scene.lightModulator();
        ImGui::End();
    }

//    void treeMesh(Env& env, int index) {
//        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
//        static bool align_label_with_current_x_position = false;
//        Mesh *mesh = env.scene.meshes[index];
//        std::string name = "Faces Mesh " + std::to_string(index);
//        if (ImGui::TreeNode(name.c_str()))
//        {
//            static int selection_mask = (1 << 2);
//            int node_clicked = -1;
//            for (int i = 0; i < mesh->faces.size(); i++)
//            {
//                ImGuiTreeNodeFlags node_flags = base_flags;
//                const bool is_selected = (selection_mask & (1 << i)) != 0;
//                if (is_selected)
//                    node_flags |= ImGuiTreeNodeFlags_Selected;
//                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
//                ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "> %s %d", "Face", i);
//                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
//                    node_clicked = i;
//                    env.scene.change_focus(mesh, mesh->faces[i]);
//                    env.render();
//                    // BEHAVIOR IS HERE
//                }
//            }
//            if (node_clicked != -1)
//                selection_mask = (1 << node_clicked);
//            if (align_label_with_current_x_position)
//                ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
//            ImGui::TreePop();
//        }
//    }

//    void printObjInfo(Mesh* focus_mesh) {
//        if (focus_mesh == nullptr) {
//            ImGui::Text("No Mesh Selected");
//            return;
//        }
//        std::string text = "type : Mesh\n";
//        text += "Number of Faces : " + std::to_string(focus_mesh->faces.size()) + "\n";
//        text += "Number of Summit : " + std::to_string(focus_mesh->points.size()) + "\n";
////    text += "Summits :\n";
////    for (auto & summit : focus_mesh->points) {
////        text += summit->to_string() + "\n";
////    }
//        ImGui::Text("%s", text.c_str());
//    }
}