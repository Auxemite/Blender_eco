#pragma once

#include "vertex.hh"
#include "graphicsUtils.hh"
#include "typebuffer.hh"

class GraphicsObject {
public:
    Graphics::TypedBuffer<Engine::vertex> vertex_buffer;
    Graphics::TypedBuffer<u32> index_buffer;

    GraphicsObject() = default;
    GraphicsObject(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
    ~GraphicsObject() = default;

    void updateFromMesh(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
    void draw();
};