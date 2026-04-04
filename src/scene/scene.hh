#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

class Scene : NonMovable {
    public:
        Camera camera;

        std::vector<Mesh *> meshes;
//        std::vector<Light *> lights;
        Light *light = nullptr;

        std::vector<Material *> materials;
        std::vector<const char*> materialNames;

        std::vector<Texture *> textures;

        std::vector<Mesh *> selectedMeshes;
    //    Mesh *xArrow = new Mesh("../data/xArrow.obj");
    //    Mesh *yArrow = new Mesh("../data/yArrow.obj");
    //    Mesh *zArrow = new Mesh("../data/zArrow.obj");

        glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
        Modifier modifier;

        // Flags
        bool textureEnabled = false;

        Scene();
        ~Scene();

        void drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
        void drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
        void drawOutline(unsigned int shaderProgram);

        void addMesh(const std::string& pathName);
        void deleteMesh(int meshIndex);
        void duplicateMesh(int meshIndex);

        void addMaterial();
        void addTexture(const std::string& pathName);
        void deleteMaterial(int materialIndex);
        void deleteTexture(int textutreIndex);

        void linkMaterialToMesh(int meshIndex, int materialIndex);
        void linkTextureToMesh(int meshIndex, int textureIndex);

        void clearSelectedMeshList();
};