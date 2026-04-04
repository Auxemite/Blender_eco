#pragma once

#include <vector>
#include "scene/mesh.hh"
#include "scene/camera.hh"
#include "scene/scene.hh"

enum EditmodeType {
    VERTEX = 1,
    EDGE = 2,
    FACE = 0,
};

class EditModeScene : NonMovable {
    public:
        std::vector<Mesh *> selectedMeshes_;
        Camera camera_;

        glm::vec3 sceneCenter_ = glm::vec3(0.0f, 0.0f, 0.0f);
        Modifier modifier_ = {};

        EditmodeType editmodeType_ = FACE;
        bool shiftMode_ = false;

        EditModeScene() = delete;
        explicit EditModeScene(const Scene& scene);
        ~EditModeScene() = default;

        void updateSceneCenter();
        void synchronizeScene(const Scene& scene);

        void drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
};


void toggleEditMode(Scene *scene, EditModeScene *editModeScene);