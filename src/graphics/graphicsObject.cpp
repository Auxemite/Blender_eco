#include "graphicsObject.hh"

GraphicsObject::GraphicsObject(std::vector<Engine::vertex> vertices, std::vector<u32> indices) {
    this->eboSize = static_cast<int>(indices.size()); // Warning : conversion from long long to int
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    // Send data to buffer
    glNamedBufferData(VBO, sizeof(Engine::vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(EBO, sizeof(u32) * indices.size(),  indices.data(),  GL_STATIC_DRAW);

    // Bind VBO and EBO to VAO
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Engine::vertex));
    glVertexArrayElementBuffer(VAO, EBO);

    // Def attributes
    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glEnableVertexArrayAttrib(VAO, 2);
    glEnableVertexArrayAttrib(VAO, 3);
    glEnableVertexArrayAttrib(VAO, 4);

    // Position
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    // Normal
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 1, 0);

    // UV
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 2, 0);

    // Tangent + bitangent sign
    glVertexArrayAttribFormat(VAO, 3, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 3, 0);

    // Color
    glVertexArrayAttribFormat(VAO, 4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 4, 0);
}

GraphicsObject::~GraphicsObject() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void GraphicsObject::updateVBOFromMesh(std::vector<Engine::vertex> vertices) {
    glNamedBufferSubData(VBO, 0, sizeof(Engine::vertex) * vertices.size(), vertices.data());
}

void GraphicsObject::updateEBOFromMesh(std::vector<u32> indices) {
    glNamedBufferSubData(EBO, 0, sizeof(u32) * indices.size(),  indices.data());
}

void GraphicsObject::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, eboSize, GL_UNSIGNED_INT, nullptr);
}