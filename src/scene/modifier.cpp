#include "modifier.hh"
#include "imgui.h"

void Modifier::clear() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = 1.0f;
}

bool Modifier::isCleared() const {
    return position == glm::vec3(0.0f) && rotation == glm::vec3(0.0f) && scale == 1.0f;
}


void Modifier::positionModulator() {
    ImGui::SliderFloat("Position X", &position.x, -5, 5);
    ImGui::SliderFloat("Position Y", &position.y, -5, 5);
    ImGui::SliderFloat("Position Z", &position.z, -5, 5);
}

void Modifier::rotationModulator() {
    ImGui::SliderFloat("Rotation X", &rotation.x, -5.0f, 5.0f);
    ImGui::SliderFloat("Rotation Y", &rotation.y, -5.0f, 5.0f);
    ImGui::SliderFloat("Rotation Z", &rotation.z, -5.0f, 5.0f);
}

void Modifier::scaleModulator() {
    ImGui::SliderFloat("Scale", &scale, 0.1f, 5.0f);
}