#pragma once

#include "vertex.hh"
#include "graphicsUtils.hh"

class GraphicsObject : NonMovable {
public:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int eboSize;

    void setup(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
    GraphicsObject() = default;
    ~GraphicsObject() = default;

//    void updateFromMesh(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
    void draw();
};