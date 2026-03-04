#pragma once

#include "vertex.hh"
#include "graphicsUtils.hh"

class GraphicsObject : NonMovable {
public:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    int eboSize = 0;

    void setup(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
    GraphicsObject() = default;
    ~GraphicsObject() = default;

    void updateVBOFromMesh(std::vector<Engine::vertex> vertices);
    void updateEBOFromMesh(std::vector<u32> indices);
    void draw();
};