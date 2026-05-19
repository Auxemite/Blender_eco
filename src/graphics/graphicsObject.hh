#pragma once

#include "vertex.hh"
#include "utils.hh"
#include "texture.hh"
#include <glad/gl.h>

class GraphicsObject : NonMovable {
    public:
        GraphicsObject() = delete;
        explicit GraphicsObject(std::vector<Engine::vertex> vertices);
        ~GraphicsObject();

        bool isLinkedTexture(Texture *texture) const;
        void linkToGraphicObject(Texture *_texture);

        void updateVBOFromMesh(std::vector<Engine::vertex> vertices);

        void draw();

    private:
        GLuint VAO_ = 0;
        GLuint VBO_ = 0;
        int vboSize_ = 0;
        Texture *texture_ = nullptr;
};