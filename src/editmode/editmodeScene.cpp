#include "editmodeScene.hh"
#include "graphics/uniform.hh"
#include "utils/mathUtils.hh"
#include "glad/gl.h"

using namespace EditMode;

EditModeScene::EditModeScene(const Scene &scene) {
    camera_ = scene.camera();
    selectedMeshes_ = scene.selectedMeshes();
    sceneCenter_ = glm::vec3(0.0f, 0.0f, 0.0f);
}


Camera EditModeScene::camera() const {
    return camera_;
}

void EditModeScene::applyAndUpdate() {
    if (modifier_.isCleared())
        return;

    for (auto mesh : selectedMeshes_) {
        if (!mesh->isVisible())
            continue;
        if (!mesh->isSelected())
            std::cerr << "toggleEditmode Warning : Mesh not mesh->selected false for a selected mesh\n";

        mesh->applySelectedAndUpdate(modifier_);
        mesh->clearSelectedPoints();
    }
    modifier_.clear();
}

void EditModeScene::updateSceneCenter() {
    glm::vec3 vertexSum;
    for (auto mesh : this->selectedMeshes_) {
        vertexSum += mesh->vertexMean();
    }
    sceneCenter_ = vertexSum / static_cast<float>(this->selectedMeshes_.size());
}

void EditModeScene::synchronizeScene(const Scene &scene) {
    editmodeType_ = EditmodeType::FACE;
    camera_ = scene.camera();
    selectedMeshes_ = scene.selectedMeshes();
    sceneCenter_ = glm::vec3(0.0f, 0.0f, 0.0f);;
    modifier_.clear();
}

void EditModeScene::drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, camera_);
    Uniform::setModifierUniforms(shaderProgram, this->modifier_);

    for (auto mesh : this->selectedMeshes_) {
        if (!mesh->isSelected())
            std::cerr  << "DrawSelectedMeshes Warning : Mesh should be selected\n";

        if (!mesh->isVisible())
            continue;

        Uniform::setMeshUniforms(shaderProgram, *mesh);
        mesh->draw();
    }
}

void EditModeScene::rayCasting(const glm::vec3& rayDir) {
    if (!Env::editmode || selectedMeshes_.empty())
        return;

    Mesh *mesh = selectedMeshes_[0];
    if (!mesh->isSelected())
        std::cerr << "HitMeshFaceTest Warning : Mesh should be selected\n";

    mesh->applySelectedAndUpdate(modifier_);
    modifier_.clear();

    if (!Env::shiftMode)
        mesh->clearSelectedPoints();

    if (editmodeType_ == FACE) {
        Math::RayInfo rayInfo = mesh->rayMeshIntersection(camera_.position(), rayDir);
        if (rayInfo.hitFace) {
            glm::ivec3 vertexIndexes = rayInfo.hitFace->vertexIndexes();
            std::cout << "Face touched " << vertexIndexes.x << " " << vertexIndexes.y << " " << vertexIndexes.z << "\n";
            mesh->addToSelectedPoints(vertexIndexes);
        }
        else
            std::cout << "Void Raycast\n";
    }
    else if (editmodeType_ == EDGE) {
//        Math::RayInfo rayInfo = mesh->rayMeshIntersection(camera_.position(), rayDir);
//        if (rayInfo.hitFace) {
//            glm::ivec3 vertexIndexes = rayInfo.hitFace->vertexIndexes();
//            glm::vec3 hitPoint = camera_.position() + rayDir * rayInfo.hitScalar;
//            glm::ivec2 edge = Math::getClosestEdge(hitPoint, );
//            mesh->graphicsObject->updateVBOFromMesh(mesh->verticesEditmode());
//        }
//        else
//            std::cout << "Void Raycast\n";
        std::cout << "Edge is on work in progress\n";
    }
    else { //EditmodeType::VERTEX
        int hitVertexIndex = mesh->rayMeshIntersection(camera_.position(), rayDir, 0.2f);
        if (hitVertexIndex >= 0) {
            std::cout << "Vertex touched " << hitVertexIndex << "\n";
            mesh->addToSelectedPoints(hitVertexIndex);
        }
        else
            std::cout << "Void Raycast\n";
    }

    mesh->applyEditModeSelection();
}

void EditMode::EditModeScene::processInputs(GLFWwindow *window, float deltaTime) {
    camera_.processInputs(window, deltaTime);
}

void EditMode::toggleEditMode(Scene& scene, EditModeScene& editModeScene) {
    if (Env::editmode) {
        scene.applyAndUpdate();
        editModeScene.synchronizeScene(scene);
    }
    else {
        editModeScene.applyAndUpdate();
    }
}

