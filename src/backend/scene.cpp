#include "scene.hh"

Scene::~Scene() {
    for (auto mesh : this->meshes) {
        delete mesh;
    }
    this->meshes.clear();

    for (auto light : this->lights) {
        delete light;
    }
    this->lights.clear();

    for (auto mesh : this->materials) {
        delete mesh;
    }
    this->materials.clear();
}

void Scene::addMesh(const std::string& filename) {
    Mesh *mesh = new Mesh(filename);
    meshes.push_back(mesh);
}

void Scene::deleteMesh(int meshIndex) {
    meshes.erase(meshes.begin() + meshIndex);
}

void Scene::duplicateMesh(int meshIndex) {
    Mesh *mesh = new Mesh(*meshes[meshIndex]);
    meshes.push_back(mesh);
}

void Scene::toggleEditmode(bool _editmode, EditmodeType _editmodeType) {
    editmode = _editmode;
    if (editmode) {
        editmodeType = _editmodeType;
        for (auto mesh : meshes) {
            if (mesh->selected) {
                selectedMeshes.push_back(mesh);
            }
        }
    }
    else {
        editmodeType = NO_EDITMODE;
        selectedMeshes.clear();
        selectedPoints.clear();
    }
}