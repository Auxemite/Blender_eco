#include "scene.hh"
#include "graphics/uniform.hh"

Scene::Scene() {
    Texture *defaultTexture = new Texture();
    defaultTexture->setName("default_texture");
    textures.push_back(defaultTexture); // Default white texture
    textureNames.push_back(defaultTexture->name());
    light = new Light(LightType::PointLight,
              glm::vec3(5.0f, 5.0f, 5.0f),
              glm::vec3(1.0f, 1.0f, 0.5f),
              50.0f);
}

Scene::~Scene() {
    for (auto mesh : this->meshes) {
        delete mesh;
    }
    this->meshes.clear();

//    for (auto light : this->lights) {
//        delete light;
//    }
//    this->lights.clear();
    delete light;

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
    if (this->textureEnabled)
        Uniform::setLightUniforms(shaderProgram, light);

    for (auto mesh : this->selectedMeshes) {
        if (!mesh->selected)
            std::cerr  << "DrawSelectedMeshes Warning : Mesh should be selected\n";

        if (!mesh->is_visible)
            continue;

        Uniform::setMeshUniforms(shaderProgram, mesh);
        if (textureEnabled)
            Uniform::setMaterialAndTextureUniforms(shaderProgram, mesh);
        mesh->graphicsObject->draw();
    }
}

void Scene::drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor) {
    glUseProgram(shaderProgram);
    Uniform::setUniqueColorUniforms(shaderProgram, unicolor);
    Uniform::setBasicUniforms(shaderProgram, &this->camera);
    if (this->textureEnabled)
        Uniform::setLightUniforms(shaderProgram, this->light);

    for (auto mesh : this->meshes) {
        if (mesh->selected || !mesh->is_visible)
            continue;

        Uniform::setModifierUniforms(shaderProgram,{});
        Uniform::setMeshUniforms(shaderProgram, mesh);
        if (textureEnabled)
            Uniform::setMaterialAndTextureUniforms(shaderProgram, mesh);
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

void Scene::addMesh(const std::string& pathName) {
    Mesh *mesh = new Mesh(pathName);
    mesh->graphicsObject->linkTextureToMesh(this->textures[0]);
    meshes.push_back(mesh);
}

void Scene::deleteMesh(int meshIndex) {
    meshes.erase(meshes.begin() + meshIndex);
}

void Scene::duplicateMesh(int meshIndex) {
    Mesh *mesh = new Mesh(*meshes[meshIndex]);
    mesh->graphicsObject->linkTextureToMesh(this->textures[0]);
    meshes.push_back(mesh);
}

void Scene::addMaterial() {
    Material *material = new Material(MATERIAL_TYPE::PBR);
    materials.push_back(material);
    materialNames.push_back(material->name());
}

void Scene::addTexture(const std::string& pathName) {
    Texture *texture = new Texture(pathName);
    textures.push_back(texture);
    textureNames.push_back(texture->name());
}

void Scene::deleteMaterial(int materialIndex) {
    // TODO unlink every mesh linked to this material
    materials.erase(materials.begin() + materialIndex);
    materialNames.erase(materialNames.begin() + materialIndex);
}

void Scene::deleteTexture(int textureIndex) {
    // TODO unlink every mesh linked to this texture
    textures.erase(textures.begin() + textureIndex);
    textureNames.erase(textureNames.begin() + textureIndex);
}

void Scene::linkMaterialToMesh(int meshIndex, int materialIndex) {
    if (meshIndex < meshes.size() && materialIndex < materials.size())
        this->meshes[meshIndex]->material = this->materials[materialIndex];
    else
        std::cerr << "linkMaterialToMesh Error : meshIndex or materialIndex invalid\n";
}

void Scene::linkMaterialToSelectedMesh(int meshIndex, int materialIndex) {
    if (meshIndex < selectedMeshes.size() && materialIndex < materials.size())
        this->selectedMeshes[meshIndex]->material = this->materials[materialIndex];
    else
        std::cerr << "linkMaterialToMesh Error : meshIndex or materialIndex invalid\n";
}

void Scene::linkTextureToMesh(int meshIndex, int textureIndex) {
    if (meshIndex < meshes.size() && textureIndex < textures.size())
        this->meshes[meshIndex]->graphicsObject->linkTextureToMesh(textures[textureIndex]);
    else
        std::cerr << "linkTextureToMesh Error : meshIndex or textureIndex invalid\n";
}

void Scene::linkTextureToSelectedMesh(int meshIndex, int textureIndex) {
    if (meshIndex < selectedMeshes.size() && textureIndex < textures.size())
        this->selectedMeshes[meshIndex]->graphicsObject->linkTextureToMesh(textures[textureIndex]);
    else
        std::cerr << "linkTextureToMesh Error : meshIndex or textureIndex invalid\n";
}

void Scene::clearSelectedMeshList() {
    for (auto mesh : this->selectedMeshes) {
        mesh->selected = false;
    }
    this->selectedMeshes.clear();
}