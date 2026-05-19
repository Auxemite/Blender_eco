#include "modifier.hh"
#include "imgui.h"

void Modifier::clear() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

bool Modifier::isCleared() const {
    return position == glm::vec3(0.0f) && rotation == glm::vec3(0.0f) && scale == glm::vec3(1.0f);
}

void Modifier::positionModulator() {
    ImGui::Text("Position");
    ImGui::SliderFloat("X##modifierPositionX", &position.x, -5, 5);
    ImGui::SliderFloat("Y##modifierPositionY", &position.y, -5, 5);
    ImGui::SliderFloat("Z##modifierPositionZ", &position.z, -5, 5);
}

void Modifier::rotationModulator() {
    ImGui::Text("Rotation");
    ImGui::SliderFloat("X##modifierRotationX", &rotation.x, -5.0f, 5.0f);
    ImGui::SliderFloat("Y##modifierRotationY", &rotation.y, -5.0f, 5.0f);
    ImGui::SliderFloat("Z##modifierRotationZ", &rotation.z, -5.0f, 5.0f);
}

void Modifier::scaleModulator() {
    ImGui::Text("Scale");
    ImGui::SliderFloat("X##modifierScaleX", &scale.x,  0.1f, 5.0f);
    ImGui::SliderFloat("Y##modifierScaleY", &scale.y,  0.1f, 5.0f);
    ImGui::SliderFloat("Z##modifierScaleZ", &scale.z,  0.1f, 5.0f);
}