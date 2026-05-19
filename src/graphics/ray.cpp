#include <vector>

#include "imgui/imgui.h"
#include "ray.hh"
#include "uniform.hh"
#include "utils/mathUtils.hh"

Ray::Ray(glm::vec3 cameraPos) {
#ifdef RAY_VISIBLE
    std::vector<float> rayVertices = generateRay(glm::vec3(0.0f, 0.0f, 0.0f), cameraPos);
    raySize_ = rayVertices.size();
    glCreateVertexArrays(1, &rayVAO_);
    glCreateBuffers(1, &rayVBO_);

    glNamedBufferData(rayVBO_, raySize_ * sizeof(float), rayVertices.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(rayVAO_, 0, rayVBO_, 0, 6 * sizeof(float));

    glEnableVertexArrayAttrib(rayVAO_, 0);
    glEnableVertexArrayAttrib(rayVAO_, 1);

    glVertexArrayAttribFormat(rayVAO_, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(rayVAO_, 0, 0);

    glVertexArrayAttribFormat(rayVAO_, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(rayVAO_, 1, 0);
#endif
}

void Ray::rayCasting(Scene& scene, float width, float height) {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 imageMin = ImGui::GetItemRectMin();

    // Image position
    float localX = mousePos.x - imageMin.x;
    float localY = mousePos.y - imageMin.y;
    localX = glm::clamp(localX, 0.0f, width);
    localY = glm::clamp(localY, 0.0f, height);

    // Ray creation
    const Camera camera = scene.camera();
    glm::vec3 rayDir = camera.getMouseRay(localX,localY,width,height);
    scene.rayCasting(rayDir);

#ifdef RAY_VISIBLE
    std::vector<float> rayVertices = generateRay(rayDir, scene.camera().position());
    raySize_ = rayVertices.size();
    glNamedBufferSubData(rayVBO_, 0, raySize_ * sizeof(float), rayVertices.data());
#endif
}

void Ray::draw(unsigned int shaderProgram, const Camera& camera) {
#ifdef RAY_VISIBLE
    glUseProgram(shaderProgram);
    Uniform::setBasicUniforms(shaderProgram, camera);

    glBindVertexArray(rayVAO_);
    glDrawArrays(GL_LINES, 0, raySize_ / 6); // Warning : gridSize conversion from size_t to int
#endif
}

std::vector<float> Ray::generateRay(glm::vec3 rayDir, glm::vec3 cameraPos) {
    std::vector<float> rayVertices;
    rayVertices.reserve(12);

    rayVertices.push_back(cameraPos.x + rayDir.x * 100.0f);
    rayVertices.push_back(cameraPos.y + rayDir.y * 100.0f);
    rayVertices.push_back(cameraPos.z + rayDir.z * 100.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);

    rayVertices.push_back(cameraPos.x + rayDir.x * -100.0f);
    rayVertices.push_back(cameraPos.y + rayDir.y * -100.0f);
    rayVertices.push_back(cameraPos.z + rayDir.z * -100.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);

    return rayVertices;
}
