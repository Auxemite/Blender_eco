#pragma once

#include <string>
#include "graphicsUtils.hh"
#include "backend/camera.hh"
#include "backend/scene.hh"

namespace Graphics {
    void clearFrameBuffer();
    void audit_bindings();
    void checkOpenGLError(const std::string &message);
    void drawInterfaceObject(unsigned int shaderProgram, Scene *scene);
}