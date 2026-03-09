#include "graphics.hh"

#include "shaderUtils.hh"
#include "uniform.hh"

namespace Graphics {

void clearFrameBuffer() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void checkOpenGLError(const std::string &message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << message << "): " << err << std::endl;
    }
}

void audit_bindings() {
    auto get = [](GLenum e) {
        int v = 0;
        glGetIntegerv(e , &v);
        return v;
    };

    auto get_at = [](GLenum e, unsigned index) {
        int v = 0;
        glGetIntegeri_v(e , index, &v);
        return v;
    };

    const int current_program = get(GL_CURRENT_PROGRAM);
    ALWAYS_ASSERT(current_program && glIsProgram(current_program), "Bound program is destroyed or invalid");


    auto get_resource_count = [=](GLenum type) {
        int count = 0;
        glGetProgramInterfaceiv(current_program, type, GL_ACTIVE_RESOURCES, &count);
        return count;
    };

    auto get_resource_property = [=](GLenum res_type, GLenum property, unsigned index) {
        int value = 0;
        glGetProgramResourceiv(current_program, res_type, index, 1, &property, 1, nullptr, &value);
        return value;
    };

    {
        const int uniform_count = get_resource_count(GL_UNIFORM);
        for(int i = 0; i != uniform_count; ++i) {
            const GLenum type = get_resource_property(GL_UNIFORM, GL_TYPE, i);
        }
    }

    {
        const int block_count = get_resource_count(GL_UNIFORM_BLOCK);
        for(int i = 0; i != block_count; ++i) {
            const unsigned binding = get_resource_property(GL_UNIFORM_BLOCK, GL_BUFFER_BINDING, i);

            const int buffer = get_at(GL_UNIFORM_BUFFER_BINDING, binding);
            ALWAYS_ASSERT(buffer && glIsBuffer(buffer), "Bound uniform buffer is destroyed or invalid");

            void* mapping = nullptr;
            glGetNamedBufferPointerv(buffer, GL_BUFFER_MAP_POINTER, &mapping);
            ALWAYS_ASSERT(!mapping, "Uniform buffer is still mapped");
        }
    }

    {
        const int block_count = get_resource_count(GL_SHADER_STORAGE_BLOCK);
        for(int i = 0; i != block_count; ++i) {
            const unsigned binding = get_resource_property(GL_SHADER_STORAGE_BLOCK, GL_BUFFER_BINDING, i);

            const int buffer = get_at(GL_SHADER_STORAGE_BUFFER_BINDING, binding);
            ALWAYS_ASSERT(buffer && glIsBuffer(buffer), "Bound storage buffer is destroyed or invalid");

            void* mapping = nullptr;
            glGetNamedBufferPointerv(buffer, GL_BUFFER_MAP_POINTER, &mapping);
            ALWAYS_ASSERT(!mapping, "Storage buffer is still mapped");
        }
    }
}

void drawInterfaceObject(unsigned int shaderProgram, Scene *scene) {
//    glUseProgram(shaderProgram);
//    Uniform::setModelViewProjGui(shaderProgram, &scene->camera);
//    Uniform::setModifierUniforms(shaderProgram, scene->modifier);
//    Uniform::setUniqueColorUniforms(shaderProgram, glm::vec3(1.0, 0.0, 0.0));
//    scene->xArrow->graphicsObject->draw();
//    Uniform::setUniqueColorUniforms(shaderProgram, glm::vec3(0.0, 1.0, 0.0));
//    scene->yArrow->graphicsObject->draw();
//    Uniform::setUniqueColorUniforms(shaderProgram, glm::vec3(0.0, 0.0, 1.0));
//    scene->zArrow->graphicsObject->draw();
}

}