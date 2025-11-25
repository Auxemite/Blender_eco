#pragma once

#include "triangle.hh"
#include "material.hh"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "frontend/vertex.hh"
#include "frontend/graphicsUtils.hh"
#include "frontend/graphicsObject.hh"

class Mesh {
public:
    std::vector<glm::vec3 *> points;
    std::vector<Triangle *> faces;
    Material *material = nullptr;
    GraphicsObject *graphicsObject = nullptr;

    bool selected = false;
    bool is_visible = true;

    Mesh() = default;
    explicit Mesh(const std::string& filename); //From .obj
    std::vector<Engine::vertex> vertices();
    std::vector<u32> indices();
};