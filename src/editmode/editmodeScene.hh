#pragma once

#include <vector>
#include "scene/mesh.hh"
#include "scene/camera.hh"
#include "scene/scene.hh"

namespace EditMode {

    enum EditmodeType {
        VERTEX = 1,
        EDGE = 2,
        FACE = 0,
    };

    class EditModeScene : NonMovable {
    public:
        EditModeScene() = delete;
        explicit EditModeScene(const Scene &scene);
        ~EditModeScene() = default;

        Camera camera() const;

        void editModeModulator(Scene& scene);
        void modifierModulator();

        void applyAndUpdate();

        void updateSceneCenter();
        void synchronizeScene(const Scene &scene);
        void drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor);

        void rayCasting(const glm::vec3& rayDir);
        void processInputs(GLFWwindow *window, float deltaTime);

    private:
        std::vector<Mesh *> selectedMeshes_;
        Camera camera_;
        glm::vec3 sceneCenter_ = glm::vec3(0.0f, 0.0f, 0.0f);
        Modifier modifier_ = {};
        EditmodeType editmodeType_ = FACE;
    };

    void toggleEditMode(Scene& scene, EditModeScene& editModeScene);
}