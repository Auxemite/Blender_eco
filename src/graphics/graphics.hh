#pragma once

#include <string>
#include "utils.hh"
#include "scene/camera.hh"
#include "scene/scene.hh"

namespace Graphics {

    void clearFrameBuffer();
    void audit_bindings();
    void checkOpenGLError(const std::string &message);
    void drawInterfaceObject(unsigned int shaderProgram, const Scene& scene);

}