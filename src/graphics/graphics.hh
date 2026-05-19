#pragma once

#include <string>
#include "utils.hh"

enum class BufferUsage {
    Attribute,
    Index,
    Uniform,
    Storage,
};

namespace Graphics {
    void clearFrameBuffer();
    u32 bufferUsageToGL(BufferUsage usage);
    void audit_bindings();
    void checkOpenGLError(const std::string &message);
//    void drawInterfaceObject(unsigned int shaderProgram, const Scene& scene);
}