#pragma once

#include "glad/gl.h"

class Grid {
private:
    GLuint gridVAO = 0;
    GLuint gridVBO = 0;
    size_t gridSize = 0;

public:
    Grid();
    void draw();
};