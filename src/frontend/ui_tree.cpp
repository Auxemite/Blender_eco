#include "ui_tree.hh"

void TreeNode(Env& env) {
    ImGui::Begin("Tree");
    addMesh(env);
    if (env.scene.focus_mesh != nullptr) {
        ImGui::SameLine();
        if (ImGui::Button("Delete Mesh")) { env.delete_mesh(); }
        ImGui::SameLine();
        if (ImGui::Button("Duplicate Mesh")) { env.duplicate_mesh(); }
    }

    for (int i = 0; i < env.scene.meshes.size(); i++) {
        std::string name = "> Mesh " + std::to_string(i);
        if (ImGui::Button(name.c_str())) {
            env.scene.change_focus(env.scene.meshes[i], i);
            env.render_all();
        }
        ImGui::SameLine();
        ImGui::PushID(i);
        if (ImGui::Button(eye[env.scene.meshes[i]->watch])) {
            env.scene.meshes[i]->watch = !env.scene.meshes[i]->watch;
            env.render();
        }
        ImGui::PopID();
        ImGui::SameLine();
        treeMesh(env, i);
    }

    if (env.scene.focus_mesh != nullptr) {
        printObjInfo(env.scene.focus_mesh);
    }

    ImGui::End();
}

void treeMesh(Env& env, int index) {
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    Mesh *mesh = env.scene.meshes[index];
    std::string name = "Faces Mesh " + std::to_string(index);
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