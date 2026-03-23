#pragma once

#include "glad/gl.h"
#include "backend/camera.hh"

class VisualGrid {
    private:
        GLuint gridVAO = 0;
        GLuint gridVBO = 0;
        size_t gridSize = 0;

    public:
        bool activateGrid = true;
        VisualGrid();
        void draw(unsigned int shaderProgram, Camera *camera);
};

std::vector<float> generateGrid(int gridSize);