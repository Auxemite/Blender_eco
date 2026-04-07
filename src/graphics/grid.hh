#pragma once

#include "scene/camera.hh"

class VisualGrid {
    public:
        bool activateGrid_ = true;

        VisualGrid();
        void draw(unsigned int shaderProgram, const Camera& camera);

    private:
        GLuint gridVAO_ = 0;
        GLuint gridVBO_ = 0;
        size_t gridSize_ = 0;
};

std::vector<float> generateGrid(int gridSize);