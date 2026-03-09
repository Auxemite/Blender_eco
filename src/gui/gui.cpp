#include <string>
#include <filesystem>
#include "gui.hh"
#include "backend/scene.hh"
#include "shaderUtils.hh"
namespace fs = std::filesystem;

namespace Gui {
    void mainGui(Scene *scene) {
//        ImGui::ShowDemoWindow();
        ImGui::Begin("Viewport");
        if (ImGui::Button("Basic"))
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/basic");
        if (ImGui::Button("Wireframe"))
            Env::mainShaderProgram = Shader::createShaderProgram("../shaders/wireframe");
        ImGui::SliderFloat("Position X", &scene->modifier.position.x, -5, 5);
        ImGui::SliderFloat("Position Y", &scene->modifier.position.y, -5, 5);
        ImGui::SliderFloat("Position Z", &scene->modifier.position.z, -5, 5);

        ImGui::SliderFloat("Rotation X", &scene->modifier.rotation.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation Y", &scene->modifier.rotation.y, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation Z", &scene->modifier.rotation.z, -5.0f, 5.0f);

        ImGui::SliderFloat("Scale", &scene->modifier.scale, 0.1f, 5.0f);
        ImGui::End();

        meshTreeNode(scene);
    }

    void meshTreeNode(Scene *scene) {
        ImGui::Begin("Tree");
        addMesh(scene);
        ImGui::SameLine();
        deleteMesh(scene);
        ImGui::SameLine();
        duplicateMesh(scene);

        bool appliedModifier = false;
        for (int i = 0; i < scene->meshes.size(); i++) {
            std::string name = "> Mesh " + std::to_string(i);
            if (ImGui::Button(name.c_str())) {
                if (scene->meshes[i]->selected) {
                    scene->meshes[i]->applyAndUpdate(scene->modifier);
                    appliedModifier = true;
                }

                scene->meshes[i]->selected = !scene->meshes[i]->selected;
            }
            ImGui::SameLine();
            ImGui::PushID(i);
            if (scene->meshes[i]->is_visible) {
                if (ImGui::Button("<O>")) {
                    scene->meshes[i]->is_visible = false;
                }
            }
            else {
                if (ImGui::Button("<Ø>")) {
                    scene->meshes[i]->is_visible = true;
                }
            }
            ImGui::PopID();
//            ImGui::SameLine();
//            treeMesh(env, i);
        }
        if (appliedModifier)
            scene->modifier.clear();

        ImGui::End();
    }

    void addMesh(Scene *scene) {
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
                    scene->addMesh("../data/" + filename + ".obj");
                }
            }
            ImGui::EndPopup();
        }
    }

    void deleteMesh(Scene *scene) {
        if (ImGui::Button("Delete"))
        {
            std::vector<int> meshIndexes;
            for (int i = 0; i < scene->meshes.size(); ++i) {
                if (scene->meshes[i]->selected)
                    meshIndexes.push_back(i);
            }
            for (int meshIndex : meshIndexes) {
                scene->deleteMesh(meshIndex);
            }
        }
    }

    void duplicateMesh(Scene *scene) {
        if (ImGui::Button("Duplicate"))
        {
            std::vector<int> meshIndexes;
            for (int i = 0; i < scene->meshes.size(); ++i) {
                if (scene->meshes[i]->selected)
                    meshIndexes.push_back(i);
            }
            for (int meshIndex : meshIndexes) {
                scene->duplicateMesh(meshIndex);
            }
        }
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

    void printObjInfo(Mesh* focus_mesh) {
        if (focus_mesh == nullptr) {
            ImGui::Text("No Mesh Selected");
            return;
        }
        std::string text = "type : Mesh\n";
        text += "Number of Faces : " + std::to_string(focus_mesh->faces.size()) + "\n";
        text += "Number of Summit : " + std::to_string(focus_mesh->points.size()) + "\n";
//    text += "Summits :\n";
//    for (auto & summit : focus_mesh->points) {
//        text += summit->to_string() + "\n";
//    }
        ImGui::Text("%s", text.c_str());
    }
}