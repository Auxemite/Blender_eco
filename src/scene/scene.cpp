#include "scene.hh"
#include "graphics/uniform.hh"

Scene::Scene() {
    Texture *defaultTexture = new Texture();
    defaultTexture->setName("default_texture");
    textures_.push_back(defaultTexture); // Default white texture
    textureNames_.push_back(defaultTexture->name());
}

Scene::~Scene() {
    for (auto mesh : meshes_) {
        delete mesh;
    }
    meshes_.clear();

    for (auto mesh : materials_) {
        delete mesh;
    }
    materials_.clear();
}

Camera Scene::camera() const {
    return camera_;
}

std::vector<Mesh *> Scene::selectedMeshes() const {
    return selectedMeshes_;
}

void Scene::applyAndUpdate() {
    if (modifier_.isCleared())
        return;

    for (auto mesh : selectedMeshes_) {
        if (!mesh->isVisible())
            continue;

        mesh->applyAndUpdate(modifier_);
    }
    modifier_.clear();

    if (selectedMeshes_.size() > 1)
        std::cerr << "toggleEditmode Warning : SelectedMeshes list is above 1. Multiple Selection not implemented\n";
}

void Scene::drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, camera_);
    Uniform::setModifierUniforms(shaderProgram, this->modifier_);
    if (Env::PBREnabled) {
        Uniform::setLightUniform(shaderProgram, lights_.getModifyingLight(), lights_.count());
        lights_.bind(1);
    }

    for (auto mesh : this->selectedMeshes_) {
        if (!mesh->isSelected())
            std::cerr  << "DrawSelectedMeshes Warning : Mesh should be selected\n";

        if (!mesh->isVisible())
            continue;

        Uniform::setMeshUniforms(shaderProgram, *mesh);
        if (Env::textureEnabled)
            Uniform::setMaterialAndTextureUniforms(shaderProgram, *mesh);
        mesh->draw();
    }
}

void Scene::drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, camera_);
    if (Env::PBREnabled) {
        Uniform::setLightUniform(shaderProgram, lights_.getModifyingLight(), lights_.count());
        lights_.bind(1);
    }

    for (auto mesh : this->meshes_) {
        if (mesh->isSelected() || !mesh->isVisible())
            continue;

        Uniform::setModifierUniforms(shaderProgram,{});
        Uniform::setMeshUniforms(shaderProgram, *mesh);
        if (Env::textureEnabled)
            Uniform::setMaterialAndTextureUniforms(shaderProgram, *mesh);
        mesh->draw();
    }
}

void Scene::drawOutline(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);
    Modifier outlineModifier = this->modifier_;
    outlineModifier.scale += camera_.ratioZoom();
    Uniform::setBasicUniforms(shaderProgram, camera_);
    Uniform::setModifierUniforms(shaderProgram, outlineModifier);
    Uniform::setUniqueColorUniforms(shaderProgram, glm::vec3(1.0, 1.0, 0.0));
    for (auto mesh: this->meshes_) {
        if (!mesh->isSelected() || !mesh->isVisible())
            continue;

        Uniform::setMeshUniforms(shaderProgram, *mesh);
        mesh->draw();
    }
}

void Scene::addMesh(const std::string& pathName) {
    Mesh *mesh = new Mesh(pathName);
    mesh->linkToMesh(this->textures_[0]);
    meshesNames_.push_back(mesh->name());
    meshes_.push_back(mesh);
}

void Scene::deleteMesh(int meshIndex) {
    meshes_.erase(meshes_.begin() + meshIndex);
    meshesNames_.erase(meshesNames_.begin() + meshIndex);
}

void Scene::duplicateMesh(int meshIndex) {
    Mesh *mesh = new Mesh(*meshes_[meshIndex]);
    mesh->linkToMesh(this->textures_[0]);
    meshesNames_.push_back(mesh->name());
    meshes_.push_back(mesh);
}

void Scene::addMaterial(const glm::vec3& color, const glm::vec2& pbrFactor) {
    Material *material = new Material(MATERIAL_TYPE::PBR, color, pbrFactor);
    materials_.push_back(material);
    materialNames_.push_back(material->name());
}

void Scene::addTexture(const std::string& pathName) {
    Texture *texture = new Texture(pathName);
    textures_.push_back(texture);
    textureNames_.push_back(texture->name());
}

void Scene::deleteMaterial(int materialIndex) {
    // TODO unlink every mesh linked to this material
    materials_.erase(materials_.begin() + materialIndex);
    materialNames_.erase(materialNames_.begin() + materialIndex);
}

void Scene::deleteTexture(int textureIndex) {
    // TODO unlink every mesh linked to this texture
    textures_.erase(textures_.begin() + textureIndex);
    textureNames_.erase(textureNames_.begin() + textureIndex);
}

void Scene::linkMaterialToMesh(int meshIndex, int materialIndex) {
    if (meshIndex < meshes_.size() && materialIndex < materials_.size())
        this->meshes_[meshIndex]->linkToMesh(materials_[materialIndex]);
    else
        std::cerr << "linkMaterialToMesh Error : meshIndex or materialIndex invalid\n";
}

void Scene::linkMaterialToSelectedMesh(int meshIndex, int materialIndex) {
    if (meshIndex < selectedMeshes_.size() && materialIndex < materials_.size())
        this->selectedMeshes_[meshIndex]->linkToMesh(materials_[materialIndex]);
    else
        std::cerr << "linkMaterialToMesh Error : meshIndex or materialIndex invalid\n";
}

void Scene::linkTextureToMesh(int meshIndex, int textureIndex) {
    if (meshIndex < meshes_.size() && textureIndex < textures_.size())
        this->meshes_[meshIndex]->linkToMesh(textures_[textureIndex]);
    else
        std::cerr << "linkTextureToMesh Error : meshIndex or textureIndex invalid\n";
}

void Scene::linkTextureToSelectedMesh(int meshIndex, int textureIndex) {
    if (meshIndex < selectedMeshes_.size() && textureIndex < textures_.size())
        this->selectedMeshes_[meshIndex]->linkToMesh(textures_[textureIndex]);
    else
        std::cerr << "linkTextureToMesh Error : meshIndex or textureIndex invalid\n";
}

void Scene::clearSelectedMeshList() {
    for (auto mesh : this->selectedMeshes_) {
        mesh->setSelection(false);
    }
    this->selectedMeshes_.clear();
}

void Scene::rayCasting(const glm::vec3& rayDir) {
    float closestHitDistance = 0.0f;
    Mesh *hitMesh = nullptr;
    for (auto mesh : meshes_) {
        if (!mesh->isVisible())
            continue;

        if (mesh->isSelected())
            mesh->applyAndUpdate(modifier_);

        mesh->setSelection(false);
        Math::RayInfo rayInfo = mesh->rayMeshIntersection(camera_.position(), rayDir);
        if (rayInfo.hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > rayInfo.hitScalar)) {
            closestHitDistance = rayInfo.hitDistance;
            hitMesh = mesh;
        }
    }

    clearSelectedMeshList();
    if (hitMesh) {
        hitMesh->setSelection(true);
        selectedMeshes_.push_back(hitMesh);
        std::cout << "Hit Mesh\n";
    }
    else
        std::cout << "Void Raycast\n";

    modifier_.clear();
}


void Scene::processInputs(GLFWwindow *window, float deltaTime) {
    camera_.processInputs(window, deltaTime);
}