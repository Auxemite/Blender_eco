#include "scene.hh"
#include "graphics/uniform.hh"

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

void Scene::drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, &this->camera);
    Uniform::setModifierUniforms(shaderProgram, this->modifier);
    for (auto mesh : this->meshes) {
        if (!mesh->selected || !mesh->is_visible)
            continue;

        Uniform::setMeshUniforms(shaderProgram, mesh);
        mesh->graphicsObject->draw();
    }
}

void Scene::drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, &this->camera);
    for (auto mesh : this->meshes) {
        if (this->editmode && !mesh->selected)
            continue;
        if (!mesh->is_visible)
            continue;

        if (mesh->selected)
            Uniform::setModifierUniforms(shaderProgram, this->modifier);
        else
            Uniform::setModifierUniforms(shaderProgram,{});
        Uniform::setMeshUniforms(shaderProgram, mesh);
        mesh->graphicsObject->draw();
    }
}

void Scene::drawOutline(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);
    Modifier outlineModifier = this->modifier;
    outlineModifier.scale += 0.03f;
    Uniform::setBasicUniforms(shaderProgram, &this->camera);
    Uniform::setModifierUniforms(shaderProgram, outlineModifier);
    Uniform::setUniqueColorUniforms(shaderProgram, glm::vec3(1.0, 1.0, 0.0));
    for (auto mesh: this->meshes) {
        if (!mesh->is_visible || !mesh->selected)
            continue;

        Uniform::setMeshUniforms(shaderProgram, mesh);
        mesh->graphicsObject->draw();
    }
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

void Scene::toggleEditmode() {
    editmode = !editmode;
    if (editmode) {
        editmodeType = FACE;
        for (auto mesh : meshes) {
            if (mesh->selected) {
                selectedMeshes.push_back(mesh);
            }
        }
    }
    else {
        editmodeType = NO_EDITMODE;
        selectedMeshes.clear();
    }
}