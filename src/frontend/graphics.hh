#pragma once

#include <string>
#include "graphicsUtils.hh"
#include "backend/camera.hh"
#include "backend/scene.hh"

namespace Graphics {
    inline GLuint gridVAO = 0;
    inline GLuint gridVBO = 0;
    inline size_t gridSize = 0;

    inline GLuint rayVAO = 0;
    inline GLuint rayVBO = 0;
    inline size_t raySize = 0;
    inline bool loadedRay = false;

    void bindAndClearWindow();
    void audit_bindings();
    void checkOpenGLError(const std::string &message);

    void loadGrid();
    void loadRay(glm::vec3 rayDirection, Camera *camera);

    void drawGrid(unsigned int shaderProgram, Camera *camera);
    void drawRay(unsigned int shaderProgram, Camera *camera);
    void drawInterfaceObject(unsigned int shaderProgram, Scene *scene);
}