#pragma once

#include "triangle.hh"
#include "material.hh"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "graphics/vertex.hh"
#include "graphics/graphicsUtils.hh"
#include "graphics/graphicsObject.hh"
#include "modifier.hh"

class Mesh {
public:
    std::vector<glm::vec3 *> points;
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
    std::vector<u32> indices();

    bool rayIntersection(const glm::vec3& cam_position, const glm::vec3& direction);

    // Apply & Udpate
    void update();
    void applyAndUpdate(const Modifier& modifier);

};