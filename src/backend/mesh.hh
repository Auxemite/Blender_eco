#pragma once

#include "triangle.hh"
#include "material.hh"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

class Mesh {
public:
    std::vector<glm::vec3 *> points;
    std::vector<Triangle *> faces;
    Material *material = nullptr;
    bool selected;
    bool is_visible = true;

    Mesh() = default;
    explicit Mesh(const std::string& filename); //From .obj
};