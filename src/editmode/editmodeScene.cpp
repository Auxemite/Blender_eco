#include "editmodeScene.hh"
#include "graphics/uniform.hh"

EditModeScene::EditModeScene(const Scene &scene) {
    camera_ = scene.camera;
    selectedMeshes_ = scene.selectedMeshes;
    sceneCenter_ = scene.sceneCenter;
}

void EditModeScene::updateSceneCenter() {
    glm::vec3 spaceSum;
    float nbVextex = 0;
    for (auto mesh : this->selectedMeshes_) {
        nbVextex += static_cast<float>(mesh->points.size());
        for (auto vertex : mesh->points)
            spaceSum += *vertex;
    }
    sceneCenter_ = spaceSum / nbVextex;
}

void EditModeScene::synchronizeScene(const Scene &scene) {
    editmodeType_ = EditmodeType::FACE;
    camera_ = scene.camera;
    selectedMeshes_ = scene.selectedMeshes;
    sceneCenter_ = scene.sceneCenter;
    modifier_.clear();
}

void EditModeScene::drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, &this->camera_);
    Uniform::setModifierUniforms(shaderProgram, this->modifier_);

    for (auto mesh : this->selectedMeshes_) {
        if (!mesh->selected)
            std::cerr  << "DrawSelectedMeshes Warning : Mesh should be selected\n";

        if (!mesh->is_visible)
            continue;

        Uniform::setMeshUniforms(shaderProgram, mesh);
        mesh->graphicsObject->draw();
    }
}

void toggleEditMode(Scene *scene, EditModeScene *editModeScene) {
    if (Env::editmode) {
        for (auto mesh : scene->selectedMeshes) {
            if (!mesh->is_visible)
                continue;

            mesh->applyAndUpdate(scene->modifier);
        }
        scene->modifier.clear();

        if (scene->selectedMeshes.size() > 1)
            std::cerr << "toggleEditmode Warning : SelectedMeshes list is above 1. Multiple Selection not implemented\n";

        editModeScene->synchronizeScene(*scene);
    }
    else {
        for (auto mesh : editModeScene->selectedMeshes_) {
            if (!mesh->is_visible)
                continue;
            if (!mesh->selected)
                std::cerr << "toggleEditmode Warning : Mesh not mesh->selected false for a selected mesh\n";

            mesh->applySelectedAndUpdate(editModeScene->modifier_);
            mesh->selectedPoints.clear();
        }
        editModeScene->modifier_.clear();
        scene->modifier.clear();
    }
}

