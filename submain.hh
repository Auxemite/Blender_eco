#pragma once
#include "src/frontend/render_utils.h"
#include "src/frontend/inputs.hh"

inline glm::vec3 cameraPos;
inline unsigned int VBO, VAO, EBO;

void load_data() {
    float vertices[] = {
            // positions                         // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 1
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // 2
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // 3
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // 4
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // 5
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // 7
    };
    unsigned int indices[] = {
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
    float vertices2[] = {
            // positions                         // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 0
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 1
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 3

            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 1
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 3

            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 1
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 2
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 5

            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,// 2
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 5
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6

            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 4
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // 5
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // 6

            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 4
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 7

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 0
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,// 4
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 7

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 0
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 3
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 7

            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 3
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 7

            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 2
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 7

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 0
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,// 4
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 5

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 0
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // 1
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 5
    };
    unsigned int indices2[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

    std::string vertexCode = readShaderSource("../src/shaders/vrtx_gray.glsl");
    std::string geometryCode = readShaderSource("../src/shaders/geo_gray.glsl");
    std::string fragmentCode = readShaderSource("../src/shaders/frag_gray.glsl");

    unsigned int vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    unsigned int geometryShader = compileShader(geometryCode, GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
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

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

        // Render the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}