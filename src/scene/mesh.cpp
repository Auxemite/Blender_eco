#include <strstream>
#include <fstream>
#include <filesystem>
#include "mesh.hh"
#include <random>

#define LINE_LEN 512
namespace fs = std::filesystem;

static int meshNumber = 0;

Mesh::Mesh(const std::string &filename) {
    // Open file as stream
    auto path = fs::current_path();
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "Error File " << filename << " could not be opened";
        return;
    }

    glm::vec3 p;
    char skip;

    while (!f.eof())
    {
        char line[LINE_LEN];
        f.getline(line, LINE_LEN);

        std::strstream s;
        s << line;
        s >> skip;

        // if first char is 'v' create associated point
        if (skip == 'v' && s.peek() == ' ')
        {
            s >> p.x >> p.y >> p.z;
            vertices_.push_back(new glm::vec3(p));
        }

        // if first char is 'f' create associated face
        if (skip == 'f')
        {
            // Handle different format such as "f 3 6 9" and "f 3/4/5 6/7/8 9/10/11"
            int f[3];
            for (int & i : f)
            {
                s >> i;
                while (s.peek() != ' ' && !s.eof())
                    s >> skip;
            }

            // point index -1 because starting index is 1 in the .obj file
            glm::ivec3 vertexIndexes = {f[0] - 1,f[1] - 1,f[2] - 1};
            faces_.push_back(new Triangle(vertexIndexes, vertices_));
        }
    }

    name_ = "Mesh_" + std::to_string(meshNumber++);
    selected_ = false;
    visible_ = true;
//    graphicsObject = new GraphicsObject(vertices(), indices());
    graphicsObject_ = new GraphicsObject(vertices());
    this->update();
}

Mesh::Mesh(const Mesh& mesh) {
    faces_.reserve(mesh.faces_.size());
    for (auto face : mesh.faces_) {
        faces_.push_back(new Triangle(face->vertexIndexes(), mesh.vertices_));
    }
    vertices_.reserve(mesh.vertices_.size());
    for (auto point : mesh.vertices_) {
        vertices_.push_back(new glm::vec3(*point));
    }

    material_ = mesh.material_;
    center_ = mesh.center_;
    name_ = "Mesh_" + std::to_string(meshNumber++);
    selected_ = false;
    visible_ = true;
//    graphicsObject = new GraphicsObject(vertices(), indices());
    graphicsObject_ = new GraphicsObject(vertices());
}

Mesh::~Mesh() {
    if (!graphicsObject_)
        std::cerr << "Mesh Destructor Warning : Mesh had no graphicsObject\n";
    else
        delete graphicsObject_;

    // materials are deleted in the scene object
    material_ = nullptr;

    for (auto face : faces_) {
        delete face;
    }
    faces_.clear();
    for (auto point : vertices_) {
        delete point;
    }
    vertices_.clear();
}

bool Mesh::isSelected() const {
    return selected_;
}

void Mesh::setSelection(bool newStatus) {
    selected_ = newStatus;
}

bool Mesh::isVisible() const {
    return visible_;
}

void Mesh::setVisibility(bool newStatus) {
    visible_ = newStatus;
}

const char* Mesh::name() const {
    return name_.c_str();
}

glm::vec3 Mesh::albedo() const {
    return material_ ? material_->color() : glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::vec2 Mesh::pbrFactor() const {
    return material_ ? material_->pbrFactor() : glm::vec2(1.0f, 0.0f);
}

glm::vec3 Mesh::center() const {
    return center_;
}

glm::vec3 Mesh::vertexMean() const {
    glm::vec3 vertexSum;
    float nbVextex = static_cast<float>(vertices_.size());
    for (auto vertex : vertices_)
        vertexSum += *vertex;
    return vertexSum / nbVextex;
}

bool Mesh::isLinkedMaterial(Material *material) const {
    return material_ != nullptr && material_ == material;
}

bool Mesh::isLinkedTexture(Texture *texture) const {
    return graphicsObject_->isLinkedTexture(texture);
}

void Mesh::draw() {
    this->graphicsObject_->draw();
}

void Mesh::linkToMesh(Texture *texture) {
    this->graphicsObject_->linkToGraphicObject(texture);
}

void Mesh::linkToMesh(Material *material) {
    this->material_ = material;
}

// TODO verify function
Math::RayInfo Mesh::rayMeshIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir) {
    float closestHitDistance = 0;
    Triangle *hitFace = nullptr;
    float hitScalar = 0;
    for (auto & face : faces_) {
        hitScalar = face->rayTriangleIntersection(vertices_, cameraPos, rayDir);
        float hitDistance = glm::length(rayDir * hitScalar);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance)) {
            closestHitDistance = hitDistance;
            hitFace = face;
        }
    }
    return { cameraPos, rayDir, hitFace, hitScalar, closestHitDistance };
}

int Mesh::rayMeshIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir, float sphereRadius) {
    int hitVertexIndex = -1;
    float closestHitDistance = 0;
    for (int i = 0; i < vertices_.size(); i++) {
        float hitDistance = Math::raySphereIntersection(cameraPos, rayDir, *vertices_[i], sphereRadius);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance)) {
            closestHitDistance = hitDistance;
            hitVertexIndex = i;
        }
    }
    return hitVertexIndex;
}

void Mesh::update() {
    // Update Mid Point
    glm::vec3 mid(0.0f, 0.0f, 0.0f);
    size_t nbPoint = vertices_.size();
    for (auto point : vertices_) {
        mid += *point;
    }
    center_ = mid / static_cast<float>(nbPoint);
}

void Mesh::applyAndUpdate(const Modifier& modifier) {
    if (modifier.isCleared())
        return;

    glm::mat3 rotationMat = Math::getRotationMatrix(modifier.rotation);
    for (auto point : vertices_) {
        *point = rotationMat * (*point - center_) * modifier.scale
        + modifier.position + center_;
    }
    graphicsObject_->updateVBOFromMesh(vertices());
    update();
}

void Mesh::applySelectedAndUpdate(const Modifier& modifier) {
    if (modifier.isCleared())
        return;

    glm::mat3 rotationMat = Math::getRotationMatrix(modifier.rotation);
    for (auto selectedIndex : selectedPoints_) {
        glm::vec3 *point = vertices_[selectedIndex];
        *point = rotationMat * (*point - center_) * modifier.scale
                 + modifier.position + center_;
    }
    graphicsObject_->updateVBOFromMesh(vertices());
    update();
}

void Mesh::applyEditModeSelection() {
    graphicsObject_->updateVBOFromMesh(verticesEditmode());
}

Engine::vertex createVertex(glm::vec3 vertexPos, glm::vec3 color, glm::vec3 normal) {
    glm::vec2 uv;
    if (normal.x > normal.y && normal.x > normal.z)
        uv = glm::vec2(vertexPos.y, vertexPos.z);
    else if (normal.y > normal.z)
        uv = glm::vec2(vertexPos.x, vertexPos.z);
    else
        uv = glm::vec2(vertexPos.x, vertexPos.y);

    return {
            vertexPos,
            glm::vec3(),
            uv,
            glm::vec4(),
            color
    };
}

std::vector<Engine::vertex> Mesh::vertices() {
    auto vertices = std::vector<Engine::vertex>();
    vertices.reserve(vertices_.size());
    for (auto & triangle : faces_) {

        glm::vec3 color = material_ == nullptr ? glm::vec3(0.0) : material_->color();
        glm::vec3 normal = abs(triangle->normal());
        glm::ivec3 vertexIndexes = triangle->vertexIndexes();

        vertices.push_back(createVertex(*vertices_[vertexIndexes.x], color, normal));
        vertices.push_back(createVertex(*vertices_[vertexIndexes.y], color, normal));
        vertices.push_back(createVertex(*vertices_[vertexIndexes.z], color, normal));
    }

//    float minX = FLT_MAX, maxX = -FLT_MAX;
//    float minZ = FLT_MAX, maxZ = -FLT_MAX;
//
//    for (const auto& v : vertices) {
//        minX = std::min(minX, v.position.x);
//        maxX = std::max(maxX, v.position.x);
//        minZ = std::min(minZ, v.position.z);
//        maxZ = std::max(maxZ, v.position.z);
//    }
//
//    for (auto& v : vertices) {
//        v.uv.x = (v.position.x - minX) / (maxX - minX);
//        v.uv.y = (v.position.z - minZ) / (maxZ - minZ);
//    }

    return vertices;
}

std::vector<Engine::vertex> Mesh::verticesEditmode() {
    auto vertices = std::vector<Engine::vertex>();
    vertices.reserve(vertices_.size());
    for (auto & triangle : faces_) {
        glm::vec3 normal = abs(triangle->normal());
        glm::ivec3 vertexIndexes = triangle->vertexIndexes();

        if (selectedPoints_.contains(vertexIndexes.x))
            vertices.push_back(createVertex(*vertices_[vertexIndexes.x], glm::vec3(1.0, 1.0, 0.0), normal));
        else
            vertices.push_back(createVertex(*vertices_[vertexIndexes.x], glm::vec3(0.0), normal));

        if (selectedPoints_.contains(vertexIndexes.y))
            vertices.push_back(createVertex(*vertices_[vertexIndexes.y], glm::vec3(1.0, 1.0, 0.0), normal));
        else
            vertices.push_back(createVertex(*vertices_[vertexIndexes.y], glm::vec3(0.0), normal));

        if (selectedPoints_.contains(vertexIndexes.z))
            vertices.push_back(createVertex(*vertices_[vertexIndexes.z], glm::vec3(1.0, 1.0, 0.0), normal));
        else
            vertices.push_back(createVertex(*vertices_[vertexIndexes.z], glm::vec3(0.0), normal));
    }

    return vertices;
}

void Mesh::clearSelectedPoints() {
    selectedPoints_.clear();
}

void Mesh::addToSelectedPoints(int value) {
    if (Env::shiftMode && selectedPoints_.contains(value))
        selectedPoints_.erase(value);
    else
        selectedPoints_.insert(value);
}