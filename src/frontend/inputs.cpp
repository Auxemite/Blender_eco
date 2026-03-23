#include "inputs.hh"

void processInput(GLFWwindow* window) {
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
//    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//        cameraDec -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedz;
//    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//        cameraDec += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedz;

    radius = glm::clamp(radius, 1.0f, 100.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
            firstMouse = true;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        float yawRad = glm::radians(yaw);
        float pitchRad = glm::radians(pitch);

        cameraFront.x = cos(yawRad) * cos(pitchRad);
        cameraFront.y = sin(pitchRad);
        cameraFront.z = sin(yawRad) * cos(pitchRad);

        cameraFront = glm::normalize(cameraFront);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= yoffset * zoom_sensitivity;
    if (radius < 1.0f)
        radius = 1.0f;
}

void UI_inputs(Env& env, const ImGuiIO& io, ImVec2 pos) {
//    float region_sz = 16.0f;
    float region_x = io.MousePos.x;// - pos.x;// - region_sz * 0.5f;
    float region_y = io.MousePos.y;// - pos.y;// - region_sz * 0.5f;
    if (region_x < 0.0f) { return; }
    else if (region_x > WIDTH) { return; }
    if (region_y < 0.0f) { return; }
    else if (region_y > HEIGHT) { return; }
    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
    ImGui::Text("Mouse down:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        if (io.MouseDownDuration[i] > 0.001)
            return;
        if (region_x >= 0.0f && region_x <= WIDTH && region_y >= 0.0f && region_y <= HEIGHT) {
            if (ImGui::IsMouseDown(i)) {
                ImGui::SameLine();
                ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
                env.update_camera();
                if (env.scene.selected_mode == 3) {
                    env.scene.select_summit(region_x, region_y);
                    env.render();
                }
                else {
                    env.scene.select_mesh(region_x, region_y);
                    env.render_all();
                }
            }
        }
    }
    ImGui::SameLine();
    ImGui::Text("Keys down:");
    struct funcs {
        static bool IsLegacyNativeDupe(ImGuiKey key) {
            return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1;
        }
    };

    auto start_key = (ImGuiKey)0;
    for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
            continue;
        ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
    }
}