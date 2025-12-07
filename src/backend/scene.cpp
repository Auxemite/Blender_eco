#include "scene.hh"

void Scene::addMesh(const std::string& filename) {
    Mesh *mesh = new Mesh(filename);
    meshes.push_back(mesh);
}