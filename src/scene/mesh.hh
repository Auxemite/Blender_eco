#pragma once

#include "triangle.hh"
#include "material.hh"

#include <iostream>
#include <vector>
#include <unordered_set>
#include "glm/glm.hpp"
#include "graphics/vertex.hh"
#include "graphics/utils.hh"
#include "graphics/graphicsObject.hh"
#include "modifier.hh"

class Mesh {
    public:
        std::vector<glm::vec3 *> points;
        std::unordered_set<int> selectedPoints;
        std::vector<Triangle *> faces;

        Material *material = nullptr;
        GraphicsObject *graphicsObject = nullptr;
        glm::vec3 midPoint = glm::vec3(0.0, 0.0, 0.0);

        bool selected = false;
        bool is_visible = true;

        Mesh() = default;
        Mesh(const Mesh& mesh);
        ~Mesh();
        Mesh& operator=(const Mesh&) = delete;

        explicit Mesh(const std::string& filename); //From .obj

        std::vector<Engine::vertex> vertices();
        std::vector<Engine::vertex> verticesEditmode();
//        std::vector<u32> indices();

        float rayIntersection(const glm::vec3& cameraPos, const glm::vec3& direction);

        // Apply & Udpate
        void update();
        void applyAndUpdate(const Modifier& modifier);
        void applySelectedAndUpdate(const Modifier& modifier);
};