#include "scene/scene.hh"

void Scene::modifierModulator() {
    modifier_.positionModulator();
    modifier_.rotationModulator();
    modifier_.scaleModulator();
}

void Scene::lightModulator() {
    lights_.lightModulator();
}

void Scene::materialModulator() {
    static int materialIndex = -1;
    if (ImGui::Button("+##addNewMaterial")) {
        addMaterial({1.0f,1.0f,1.0f}, {0.5, 0.1});
        materialIndex = static_cast<int>(materialNames_.size() - 1);
    }
    ImGui::SameLine();
    ImGui::Combo("Materials##listMaterials", &materialIndex,
                 materialNames_.data(), static_cast<int>(materialNames_.size()));
    if (materialIndex >= 0) {
        materials_[materialIndex]->colorModulator();
        materials_[materialIndex]->pbrFactorModulator();

        if (selectedMeshes_.size() == 1) { // Single selection
            if (!selectedMeshes_[0]->isLinkedMaterial(materials_[materialIndex])) {
                if (ImGui::Button("Link To Mesh##linkMaterialToSelectedMesh")) {
                    linkMaterialToSelectedMesh(0, materialIndex);
                }
            }
            else
                ImGui::Text("Linked to selected Mesh");
        }
        else if (selectedMeshes_.size() > 1) {
            std::cerr << "MaterialModulator Warning : Multiple Selected Not Implemented\n";
        }

        if (ImGui::Button("Delete##deleteMaterial")) {
            deleteMaterial(materialIndex);
            materialIndex = -1;
        }
    }
}

void Scene::textureModulator() {
    static int texturesIndex = -1;
//    if (ImGui::Button("+##addNewTexture")) {
//        addTexture();
//        texturesIndex = static_cast<int>(textureNames_.size() - 1);
//    }
//    ImGui::SameLine();
    ImGui::Combo("Textures##listMaterial", &texturesIndex,
                 textureNames_.data(), static_cast<int>(textureNames_.size()));
    if (texturesIndex >= 0) {
        if (selectedMeshes_.size() == 1) { // Single selection
            if (!selectedMeshes_[0]->isLinkedTexture(textures_[texturesIndex])) {
                if (ImGui::Button("Link To Mesh##linkTextureToSelectedMesh")) {
                    linkTextureToSelectedMesh(0, texturesIndex);
                }
            }
            else
                ImGui::Text("Linked to selected Mesh");
        }
        else if (selectedMeshes_.size() > 1) {
            std::cerr << "TextureModulator Warning : Multiple Selected Not Implemented\n";
        }

        if (ImGui::Button("Delete##deleteTexture")) {
            deleteTexture(texturesIndex);
            texturesIndex = -1;
        }
    }
}

void Scene::treeMeshModulator() {
    bool appliedModifier = false;
    for (int i = 0; i < meshes_.size(); i++) {
        if (ImGui::Button(meshes_[i]->name())) {
            if (meshes_[i]->isSelected()) {
                meshes_[i]->applyAndUpdate(modifier_);
                appliedModifier = true;
            }
            clearSelectedMeshList();
            selectedMeshes_.push_back(meshes_[i]);
            meshes_[i]->setSelection(true);
        }
        ImGui::SameLine();
        ImGui::PushID(i);
        if (meshes_[i]->isVisible()) {
            const std::string label = "<O>##unwatch" + std::to_string(i);
            if (ImGui::Button(label.c_str())) {
                meshes_[i]->setVisibility(false);
            }
        }
        else {
            const std::string label = "<Ø>##unwatch" + std::to_string(i);
            if (ImGui::Button(label.c_str())) {
                meshes_[i]->setVisibility(true);
            }
        }
        ImGui::PopID();
//            ImGui::SameLine();
//            treeMesh(env, i);
    }
    if (appliedModifier)
        modifier_.clear();
}

void Scene::duplicateMeshModulator() {
    if (ImGui::Button("Duplicate##duplicateMesh"))
    {
        if (selectedMeshes_.empty()) {
            modifier_.clear();
            return;
        }

        applyAndUpdate();
        std::vector<int> meshIndexes;
        for (int i = 0; i < meshes_.size(); ++i) {
            if (meshes_[i]->isSelected())
                meshIndexes.push_back(i);
        }
        for (int meshIndex : meshIndexes) {
            duplicateMesh(meshIndex);
        }

        selectedMeshes_.clear();
    }
}

void Scene::deleteMeshModulator() {
    if (ImGui::Button("Delete##deleteMesh"))
    {
        if (selectedMeshes_.empty()) {
            modifier_.clear();
            return;
        }

        applyAndUpdate();
        std::vector<int> meshIndexes;
        for (int i = 0; i < meshes_.size(); ++i) {
            if (meshes_[i]->isSelected())
                meshIndexes.push_back(i);
        }
        for (int meshIndex : meshIndexes) {
            deleteMesh(meshIndex);
        }

        selectedMeshes_.clear();
    }
}