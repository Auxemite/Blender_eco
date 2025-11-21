#include "graphics_utils.hh"

#include "shader_utils.hh"
#include "env.hh"
#include "vertex.hh"

namespace Graphics {

void clearWindow() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//void loadData(int mesh_index, std::vector<float> vertices, std::vector<int> indices) {
//    glGenVertexArrays(1, &(VAOs[mesh_index]));
//    glGenBuffers(1, &(VBOs[mesh_index]));
//    glGenBuffers(1, &(EBOs[mesh_index]));
//    checkOpenGLError("load_data : gen buffers");
//
//    glBindVertexArray(VAOs[mesh_index]);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh_index]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh_index]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
//
//    checkOpenGLError("load_data : bind buffers");
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    checkOpenGLError("load_data : enable attributes");
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    checkOpenGLError("after loading data");
//}

void loadData() {
    glGenVertexArrays(1, &Env::VAO);
    glGenBuffers(1, &Env::VBO);
    glGenBuffers(1, &Env::EBO);

    glBindVertexArray(Env::VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Env::VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Env::vertices), Env::vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Env::EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Env::indices), Env::indices, GL_STATIC_DRAW);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(engine::vertex), nullptr);
    // Vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(engine::vertex), (void *) (3 * sizeof(float)));
    // Vertex uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(engine::vertex),(void *) (6 * sizeof(float)));
    // Tangent / bitangent sign
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(engine::vertex), (void *) (8 * sizeof(float)));
    // Vertex color
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(engine::vertex), (void *) (12 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}