#pragma once

#include <string>
#include "graphicsUtils.hh"
#include "backend/camera.hh"

namespace Graphics {

    inline GLuint gridVAO = 0;
    inline GLuint gridVBO = 0;
    inline size_t gridSize = 0;

    u32 bufferUsageToGL(BufferUsage usage);
    void clearWindow();
    void audit_bindings();
    void checkOpenGLError(const std::string &message);
    void loadGrid();
    void drawGrid(unsigned int shaderProgram, Camera *camera);
}