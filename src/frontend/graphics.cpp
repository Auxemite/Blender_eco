#include "graphics.hh"

#include "shaderUtils.hh"
#include "env.hh"
#include "uniform.hh"

namespace Graphics {

u32 bufferUsageToGL(BufferUsage usage) {
    switch(usage) {
        case BufferUsage::Attribute:
            return GL_ARRAY_BUFFER;

        case BufferUsage::Index:
            return GL_ELEMENT_ARRAY_BUFFER;

        case BufferUsage::Uniform:
            return GL_UNIFORM_BUFFER;

        case BufferUsage::Storage:
            return GL_SHADER_STORAGE_BUFFER;
    }

    FATAL("Unknown usage value");
}

void clearWindow() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //        glBindFramebuffer(GL_FRAMEBUFFER, Gui::fbo);
    //        glViewport(0, 0, WIDTH, HEIGHT);
    //        glEnable(GL_DEPTH_TEST);
    //    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

std::vector<float> generateGrid(int gridSize) {
    std::vector<float> gridVertices;
    // Red Line for X
    gridVertices.push_back(-gridSize);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(gridSize);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    // Green Line for Z
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(-gridSize);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(gridSize);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);

    float grid_clr = 0.4f;
    for (int i = -gridSize; i <= gridSize; ++i) {
        // X
        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        // Z
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
    }

    return gridVertices;
}

void loadGrid() {
    std::vector<float> gridVertices = generateGrid(100);
    gridSize = gridVertices.size();
    glCreateVertexArrays(1, &gridVAO);
    glCreateBuffers(1, &gridVBO);

    glNamedBufferData(gridVBO, gridSize * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(gridVAO, 0, gridVBO, 0, 6 * sizeof(float));

    glEnableVertexArrayAttrib(gridVAO, 0);
    glEnableVertexArrayAttrib(gridVAO, 1);

    glVertexArrayAttribFormat(gridVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(gridVAO, 0, 0);

    glVertexArrayAttribFormat(gridVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(gridVAO, 1, 0);
}

void drawGrid(unsigned int shaderProgram, Camera *camera) {
    glUseProgram(shaderProgram);
    Uniform::setModelViewProj(shaderProgram, camera);

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridSize / 6); // Warning : gridSize conversion from size_t to int
}

}