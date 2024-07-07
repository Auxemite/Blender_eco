#pragma once
#include "render_utils.h"
#include "inputs.hh"

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float pitch = 0.0f;  // vertical angle
float lastFrame = 0.0;
unsigned int VBO, VAO, EBO;

void load_data() {
    float vertices[] = {
            // positions          // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Vertex 0: position (-0.5, -0.5, -0.5), color (1.0, 0.0, 0.0)
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // Vertex 1: position ( 0.5, -0.5, -0.5), color (0.0, 1.0, 0.0)
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Vertex 2: position ( 0.5,  0.5, -0.5), color (0.0, 0.0, 1.0)
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // Vertex 3: position (-0.5,  0.5, -0.5), color (1.0, 1.0, 0.0)
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // Vertex 4: position (-0.5, -0.5,  0.5), color (1.0, 0.0, 1.0)
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Vertex 5: position ( 0.5, -0.5,  0.5), color (0.0, 1.0, 1.0)
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // Vertex 6: position ( 0.5,  0.5,  0.5), color (1.0, 1.0, 1.0)
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f  // Vertex 7: position (-0.5,  0.5,  0.5), color (0.0, 0.0, 0.0)
    };
    unsigned int indices[] = {
            0, 1, 3,   // First triangle (face 1)
            1, 2, 3,   // Second triangle (face 1)
            1, 5, 2,   // Third triangle (face 2)
            5, 6, 2,   // Fourth triangle (face 2)
            5, 4, 6,   // Fifth triangle (face 3)
            4, 7, 6,   // Sixth triangle (face 3)
            4, 0, 7,   // Seventh triangle (face 4)
            0, 3, 7,   // Eighth triangle (face 4)
            3, 2, 7,   // Ninth triangle (face 5)
            2, 6, 7,   // Tenth triangle (face 5)
            4, 5, 0,   // Eleventh triangle (face 6)
            5, 1, 0    // Twelfth triangle (face 6)
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


void viewport_main_loop(GLFWwindow* window,  unsigned int shaderProgram) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the cube
    glUseProgram(shaderProgram);

    // Calculate the camera position using spherical coordinates
    cameraPos.x = radius * cos(glm::radians(yaw));
    cameraPos.z = radius * sin(glm::radians(yaw));
    cameraFront = glm::normalize(-cameraPos);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
//    glfwPollEvents();
}

void window2_cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}