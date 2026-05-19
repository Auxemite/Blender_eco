#include <vector>
#include <iostream>
#include "grid.hh"
#include "uniform.hh"
#include "glad/gl.h"

VisualGrid::VisualGrid() {
    std::vector<float> gridVertices = generateGrid(100);
    gridSize_ = gridVertices.size();
    glCreateVertexArrays(1, &gridVAO_);
    glCreateBuffers(1, &gridVBO_);

    glNamedBufferData(gridVBO_, gridSize_ * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(gridVAO_, 0, gridVBO_, 0, 6 * sizeof(float));

    glEnableVertexArrayAttrib(gridVAO_, 0);
    glEnableVertexArrayAttrib(gridVAO_, 1);

    glVertexArrayAttribFormat(gridVAO_, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(gridVAO_, 0, 0);

    glVertexArrayAttribFormat(gridVAO_, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(gridVAO_, 1, 0);
}

void VisualGrid::draw(unsigned int shaderProgram, const Camera& camera) {
    if (activateGrid_) {
        glUseProgram(shaderProgram);
        Uniform::setBasicUniforms(shaderProgram, camera);

        glBindVertexArray(gridVAO_);
        glDrawArrays(GL_LINES, 0, static_cast<int>(gridSize_) / 6); // Warning : gridSize conversion from size_t to int
    }
}

std::vector<float> generateGrid(int gridSize) {
    std::vector<float> gridVertices;
    gridVertices.reserve((gridSize * 2 + 1) * 24 + 24);
    // Red Line for X
    gridVertices.push_back(-static_cast<float>(gridSize));
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(1.0f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(static_cast<float>(gridSize));
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(1.0f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    // Green Line for Z
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(-static_cast<float>(gridSize));
    gridVertices.push_back(0.3f);
    gridVertices.push_back(1.0f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(static_cast<float>(gridSize));
    gridVertices.push_back(0.3f);
    gridVertices.push_back(1.0f);
    gridVertices.push_back(0.3f);

    float grid_clr = 0.4f;
    for (int i = -gridSize; i <= gridSize; ++i) {
        // X
        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        // Z
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
    }

    std::cout << "Generated grid is size " << gridVertices.size() << "\n";

    return gridVertices;
}