#include "graphicsObject.hh"
#include "glad/gl.h"

GraphicsObject::GraphicsObject(std::vector<Engine::vertex> vertices) {
    this->vboSize_ = static_cast<int>(vertices.size()); // Warning : conversion from long long to int
    glCreateVertexArrays(1, &VAO_);
    glCreateBuffers(1, &VBO_);

    // Send data to buffer
    glNamedBufferData(VBO_, sizeof(Engine::vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Bind VBO and EBO to VAO
    glVertexArrayVertexBuffer(VAO_, 0, VBO_, 0, sizeof(Engine::vertex));

    // Def attributes
    glEnableVertexArrayAttrib(VAO_, 0);
    glEnableVertexArrayAttrib(VAO_, 1);
    glEnableVertexArrayAttrib(VAO_, 2);
    glEnableVertexArrayAttrib(VAO_, 3);
    glEnableVertexArrayAttrib(VAO_, 4);

    // Position
    glVertexArrayAttribFormat(VAO_, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO_, 0, 0);

    // Normal
    glVertexArrayAttribFormat(VAO_, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(VAO_, 1, 0);

    // UV
    glVertexArrayAttribFormat(VAO_, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glVertexArrayAttribBinding(VAO_, 2, 0);

    // Tangent + bitangent sign
    glVertexArrayAttribFormat(VAO_, 3, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float));
    glVertexArrayAttribBinding(VAO_, 3, 0);

    // Color
    glVertexArrayAttribFormat(VAO_, 4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float));
    glVertexArrayAttribBinding(VAO_, 4, 0);
}

GraphicsObject::~GraphicsObject() {
    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &VAO_);
}

bool GraphicsObject::isLinkedTexture(Texture *texture) const {
    return texture_ != nullptr && texture_ == texture;
}

void GraphicsObject::linkToGraphicObject(Texture *_texture) {
    texture_ = _texture;
}

void GraphicsObject::updateVBOFromMesh(std::vector<Engine::vertex> vertices) {
    glNamedBufferSubData(VBO_, 0, sizeof(Engine::vertex) * vertices.size(), vertices.data());
}

void GraphicsObject::draw() {
    if (texture_)
        texture_->bind(0);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, vboSize_);
}