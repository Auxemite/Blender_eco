#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define WIDTH 1920
#define HEIGHT 1080

namespace Env {

inline float lastFrame = 0.0;
inline float deltaTime = 0.0f;

inline bool audit_bindings_before_draw = false;
//inline unsigned int VBO;
//inline unsigned int VAO;
//inline unsigned int EBO;

inline float vertices[] = {
        // positions                          // normals                     // UV                      // tangente / bitangante                    // colors
        -0.5f,-0.5f,-0.5f,    0.0f,0.0f,0.0f,   0.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,        1.0f, 1.0f, 1.0f,  // 0
        0.5f,-0.5f,-0.5f,     0.0f,0.0f,0.0f,   1.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,   1.0f, 1.0f, 1.0f,// 1
        0.5f,0.5f,-0.5f,  0.0f,0.0f,0.0f,   1.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,           1.0f, 1.0f, 1.0f, // 2
        -0.5f,0.5f,-0.5f,     0.0f,0.0f,0.0f,   0.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 3
        -0.5f,-0.5f,0.5f,     0.0f,0.0f,0.0f,   0.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 4
        0.5f,-0.5f,0.5f,  0.0f,0.0f,0.0f,   1.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 5
        0.5f,0.5f,0.5f,   0.0f,0.0f,0.0f,   1.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 6
        -0.5f,0.5f,0.5f,  0.0f,0.0f,0.0f,   0.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,     1.0f, 1.0f, 1.0f,  // 7
};
inline unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        1, 5, 2,
        5, 6, 2,
        5, 4, 6,
        4, 7, 6,
        4, 0, 7,
        0, 3, 7,
        3, 2, 7,
        2, 6, 7,
        4, 5, 0,
        5, 1, 0
};
inline float vertices2[] = {
        // positions                          // normals                     // UV                      // tangente / bitangante                    // colors
        -1.5f,-1.5f,-1.5f,    0.0f,0.0f,0.0f,   0.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,        1.0f, 1.0f, 1.0f,  // 0
        -0.5f,-1.5f,-1.5f,     0.0f,0.0f,0.0f,   1.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,   1.0f, 1.0f, 1.0f,// 1
        -0.5f,-0.5f,-1.5f,  0.0f,0.0f,0.0f,   1.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,           1.0f, 1.0f, 1.0f, // 2
        -1.5f,-0.5f,-1.5f,     0.0f,0.0f,0.0f,   0.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 3
        -1.5f,-1.5f,-0.5f,     0.0f,0.0f,0.0f,   0.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 4
        -0.5f,-1.5f,-0.5f,  0.0f,0.0f,0.0f,   1.0f, 0.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 5
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,0.0f,   1.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,    1.0f, 1.0f, 1.0f, // 6
        -1.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,   0.0f, 1.0f,     0.0f,0.0f,0.0f,0.0f,     1.0f, 1.0f, 1.0f,  // 7
};
inline int indicesLength = sizeof(indices) / sizeof(indices[0]);

}