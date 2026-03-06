#pragma once

#include "glad/gl.h"

class Ray {
private:
    GLuint rayVAO = 0;
    GLuint rayVBO = 0;
    size_t raySize = 0;
public:
    bool loadedRay = false;
    Ray();
    void draw();
};