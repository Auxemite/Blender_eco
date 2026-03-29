#pragma once

#include "vertex.hh"
#include "utils.hh"
#include "texture.hh"

class GraphicsObject : NonMovable {
    public:
        GLuint VAO = 0;
        GLuint VBO = 0;
        int vboSize = 0;
        Texture *texture = nullptr;

//        GraphicsObject(std::vector<Engine::vertex> vertices, std::vector<u32> indices);
        explicit GraphicsObject(std::vector<Engine::vertex> vertices);
        ~GraphicsObject();

        void linkTextureToMesh(Texture *_texture);

        void updateVBOFromMesh(std::vector<Engine::vertex> vertices);
//        void updateEBOFromMesh(std::vector<u32> indices);

        void draw();
};