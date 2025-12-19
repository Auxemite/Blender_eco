#pragma once

#include "triangle.hh"
#include "material.hh"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "frontend/vertex.hh"
#include "frontend/graphicsUtils.hh"
#include "frontend/graphicsObject.hh"

class Mesh : NonCopyable {
public:
    std::vector<glm::vec3 *> points;
    std::vector<Triangle *> faces;
    Material *material = nullptr;
    GraphicsObject *graphicsObject;

    bool selected = false;
    bool is_visible = true;

    Mesh() = default;
    explicit Mesh(const std::string& filename); //From .obj
    bool ray_intersection(const glm::vec3& cam_position, const glm::vec3& direction);
    std::vector<Engine::vertex> vertices();
    std::vector<u32> indices();
};