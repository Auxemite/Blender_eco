#pragma once
#include "src/frontend/render_utils.h"
#include "src/frontend/inputs.hh"

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastFrame = 0.0;
unsigned int VBO, VAO, EBO;

void load_data() {
    float vertices[] = {
            // positions          // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int submain2() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

    unsigned int shaderProgram = createShaderProgram("../src/shaders/vertex_shader.glsl", "../src/shaders/fragment_shader.glsl");

    load_data();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        cameraPos.x = radius * cos(glm::radians(yaw));
        cameraPos.z = radius * sin(glm::radians(yaw));
        cameraFront = glm::normalize(-cameraPos);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}