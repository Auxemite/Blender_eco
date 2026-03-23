#pragma once

#include "src/frontend/shader_utils.hh"

#define NB_SHELLS 16

inline glm::vec3 cameraPos;
inline glm::vec3 cameraFront;
inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
inline glm::vec3 cameraDec = glm::vec3(0.0f, 5.0f, 0.0f); //decalage
inline float lastFrame = 0.0;
std::vector<unsigned int> VBOs = std::vector<unsigned int>(NB_SHELLS, 0);
std::vector<unsigned int> VAOs = std::vector<unsigned int>(NB_SHELLS, 0);
std::vector<unsigned int> EBOs = std::vector<unsigned int>(NB_SHELLS, 0);
inline float deltaTime = 0.0f;
inline float radius = 10.0f;
inline float yaw = -45.0f;
inline float pitch =  0.0f;
inline int speed_rotation = 30;
inline int speed_zoom = 10;

void processInpute(GLFWwindow* window) {
    float cameraSpeedz = speed_zoom * deltaTime;
    float cameraSpeedr = speed_rotation * 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        radius -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        radius += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw += cameraSpeedr;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraDec.y += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraDec.y -= cameraSpeedz;

    radius = glm::clamp(radius, 1.0f, 100.0f);
}

void load_data() {
//    float vertices[] = {
//            // positions                           // colors                      // UV
//            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // 0
//            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 1
//            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 2
//            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // 3
//            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // 4
//            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 5
//            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
//            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // 7
//    };
//    unsigned int indices[] = {
//            0, 1, 3,
//            1, 2, 3,
//            1, 5, 2,
//            5, 6, 2,
//            5, 4, 6,
//            4, 7, 6,
//            4, 0, 7,
//            0, 3, 7,
//            3, 2, 7,
//            2, 6, 7,
//            4, 5, 0,
//            5, 1, 0
//    };
    float vertices_list[NB_SHELLS][192];
    float vertices_base[] = {
            // positions                        // colors                        // UV
            // Back
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 2
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // 3
            // Front
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 4
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 5
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // 7
            // Left
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 3
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, // 7
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // 4
            // Right
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // 1
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 2
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, // 5
            // Down
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 5
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // 4
            // Up
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, // 3
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 2
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f  // 7
    };
    for (int i = 0; i < 192; ++i) {
        if (vertices_base[i] == 0.5) {
            float scale = 0.5;
            for (auto & vertices : vertices_list) {
                scale += 0.05;
                vertices[i] = scale;
            }
        }
        else if (vertices_base[i] == -0.5) {
            float scale = -0.5;
            for (auto & vertices : vertices_list) {
                scale -= 0.05;
                vertices[i] = scale;
            }
        }
        else {
            for (auto & vertices : vertices_list) {
                vertices[i] = vertices_base[i];
            }
        }
    }
    unsigned int indices[] = {
            // Back
            0, 1, 2,
            2, 3, 0,
            // Front
            4, 5, 6,
            6, 7, 4,
            // Left
            8, 9, 10,
            10, 11, 8,
            // Right
            12, 13, 14,
            14, 15, 12,
            // Down
            16, 17, 18,
            18, 19, 16,
            // Up
            20, 21, 22,
            22, 23, 20
    };

    for (int i = 0; i < NB_SHELLS; ++i) {
        glGenVertexArrays(1, &VAOs[i]);
        glGenBuffers(1, &VBOs[i]);
        glGenBuffers(1, &EBOs[i]);

        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_list[i]), vertices_list[i], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // UV
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

int submain2() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

//    std::string vertexCode = readShaderSource("../src/shaders/normal/normal.vert");
//    std::string geometryCode = readShaderSource("../src/shaders/normal/normal.geom");
//    std::string fragmentCode = readShaderSource("../src/shaders/normal/normal.frag");

    std::string vertexCode = readShaderSource("../src/shaders/fur2/fur2.vert");
//    std::string geometryCode = readShaderSource("../src/shaders/fur2/fur2.geom");
    std::string fragmentCode = readShaderSource("../src/shaders/fur2/fur2.frag");

    unsigned int vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER, "Vertex");
//    unsigned int geometryShader = compileShader(geometryCode, GL_GEOMETRY_SHADER, "Geometry");
    unsigned int fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER, "Fragment");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program linking failed:\n" << infoLog << std::endl;
    }

    load_data();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInpute(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the grid and lines
        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

        // Calculate the camera position using spherical coordinates
        cameraPos.x = radius * cos(glm::radians(yaw));
        cameraPos.z = radius * sin(glm::radians(yaw));
        cameraPos.y = cameraDec.y;
        cameraFront = glm::normalize(-cameraPos);

        float density = 1.0f;
        // Render the cube
        for (int i = 0; i < NB_SHELLS; ++i) {
            unsigned int densityLoc = glGetUniformLocation(shaderProgram, "density");
            glUniform1f(densityLoc, density);

            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            density /= 1.5f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}