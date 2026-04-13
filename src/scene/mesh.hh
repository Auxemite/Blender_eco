#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>

#include "glm/glm.hpp"

#include "triangle.hh"
#include "material.hh"
#include "graphics/vertex.hh"
#include "graphics/utils.hh"
#include "graphics/graphicsObject.hh"
#include "modifier.hh"
#include "utils/mathUtils.hh"

class Mesh {
    public:
        Mesh() = default;
        Mesh(const Mesh& mesh);
        ~Mesh();
        Mesh& operator=(const Mesh&) = delete;

        explicit Mesh(const std::string& filename); //From .obj

        bool isSelected() const;
        void setSelection(bool newStatus);
        bool isVisible() const;
        void setVisibility(bool newStatus);
        const char* name() const;
        glm::vec3 albedo() const;
        glm::vec2 pbrFactor() const;
        glm::vec3 center() const;
        glm::vec3 vertexMean() const;
        bool isLinkedMaterial(Material *material) const;
        bool isLinkedTexture(Texture *texture) const;

        void draw();
        void linkToMesh(Texture *texture);
        void linkToMesh(Material *material);

        Math::RayInfo rayMeshIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir);
        int rayMeshIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir, float sphereRadius);

        // Apply & Udpate
        void update();
        void applyAndUpdate(const Modifier& modifier);
        void applySelectedAndUpdate(const Modifier& modifier);
        void applyEditModeSelection();

        std::vector<Engine::vertex> vertices();
        std::vector<Engine::vertex> verticesEditmode();

        void clearSelectedPoints();
        void addToSelectedPoints(const int& value);
        void addToSelectedPoints(const glm::ivec2& values);
        void addToSelectedPoints(const glm::ivec3& values);

    private:
        std::string name_;
        std::vector<glm::vec3 *> vertices_;
        std::unordered_set<int> selectedPoints_;
        std::vector<Triangle *> faces_;

        Material *material_ = nullptr;
        GraphicsObject *graphicsObject_ = nullptr;
        glm::vec3 center_ = glm::vec3(0.0, 0.0, 0.0);

        bool selected_ = false;
        bool visible_ = true;
};