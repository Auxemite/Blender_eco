#include "graphicsObject.hh"

GraphicsObject::GraphicsObject(std::vector<Engine::vertex> vertices, std::vector<u32> indices) {
    vertex_buffer = Graphics::TypedBuffer<Engine::vertex>(vertices);
    index_buffer = Graphics::TypedBuffer<u32>(indices);
}

void GraphicsObject::updateFromMesh(std::vector<Engine::vertex> vertices, std::vector<u32> indices) {
    vertex_buffer = Graphics::TypedBuffer<Engine::vertex>(vertices);
    index_buffer = Graphics::TypedBuffer<u32>(indices);
};

void GraphicsObject::draw() {
    vertex_buffer.bind(Graphics::BufferUsage::Attribute);
    index_buffer.bind(Graphics::BufferUsage::Index);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Engine::vertex), nullptr);
    // Vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Engine::vertex), reinterpret_cast<void*>(3 * sizeof(float)));
    // Vertex uv
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Engine::vertex), reinterpret_cast<void*>(6 * sizeof(float)));
    // Tangent / bitangent sign
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Engine::vertex), reinterpret_cast<void*>(8 * sizeof(float)));
    // Vertex color
    glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Engine::vertex), reinterpret_cast<void*>(12 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

//    if (audit_bindings_before_draw) {
//        audit_bindings();
//    }

    glDrawElements(GL_TRIANGLES, int(index_buffer.elementCount()), GL_UNSIGNED_INT, nullptr);
}