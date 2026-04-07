#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

class Scene : NonMovable {
    public:
        Scene();
        ~Scene();

        Camera camera() const;
        std::vector<Mesh *> selectedMeshes() const;

        // Modulators
        void modifierModulator();
        void lightModulator();
        void materialModulator();
        void textureModulator();
        void treeMeshModulator();
        void duplicateMeshModulator();
        void deleteMeshModulator();

        // Graphics
        void applyAndUpdate();
        void drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
        void drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
        void drawOutline(unsigned int shaderProgram);

        // Scene Basic Actions
        void addMesh(const std::string& pathName);
        void deleteMesh(int meshIndex);
        void duplicateMesh(int meshIndex);
        void addMaterial();
        void addTexture(const std::string& pathName);
        void deleteMaterial(int materialIndex);
        void deleteTexture(int textutreIndex);

        // Links
        void linkMaterialToMesh(int meshIndex, int materialIndex);
        void linkMaterialToSelectedMesh(int meshIndex, int materialIndex);
        void linkTextureToMesh(int meshIndex, int textureIndex);
        void linkTextureToSelectedMesh(int meshIndex, int textureIndex);

        // Others
        void clearSelectedMeshList();
        void rayCasting(const glm::vec3& rayDir);
        void processInputs(GLFWwindow *window, float deltaTime);

    private :
        Camera camera_;

        std::vector<Mesh *> meshes_;
        std::vector<Mesh *> selectedMeshes_;
        std::vector<const char*> meshesNames_;
        //        std::vector<Light *> lights_;
        Light *light_ = nullptr;

        std::vector<Material *> materials_;
        std::vector<const char*> materialNames_;

        std::vector<Texture *> textures_;
        std::vector<const char*> textureNames_;

        glm::vec3 sceneCenter_ = glm::vec3(0.0f, 0.0f, 0.0f);
        Modifier modifier_;
};