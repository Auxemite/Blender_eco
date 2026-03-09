#include <vector>

#include "glm/vec3.hpp"
#include "imgui/imgui.h"

#include "ray.hh"
#include "uniform.hh"

Ray::Ray(glm::vec3 cameraPos) {
#ifdef RAY_VISIBLE
    std::vector<float> rayVertices = generateRay(glm::vec3(0.0f, 0.0f, 0.0f), cameraPos);
    raySize = rayVertices.size();
    glCreateVertexArrays(1, &rayVAO);
    glCreateBuffers(1, &rayVBO);

    glNamedBufferData(rayVBO, raySize * sizeof(float), rayVertices.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(rayVAO, 0, rayVBO, 0, 6 * sizeof(float));

    glEnableVertexArrayAttrib(rayVAO, 0);
    glEnableVertexArrayAttrib(rayVAO, 1);

    glVertexArrayAttribFormat(rayVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(rayVAO, 0, 0);

    glVertexArrayAttribFormat(rayVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(rayVAO, 1, 0);
#endif
}

void Ray::rayCasting(Scene *scene, float width, float height) {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 imageMin = ImGui::GetItemRectMin();

    // Image position
    float localX = mousePos.x - imageMin.x;
    float localY = mousePos.y - imageMin.y;
    localX = glm::clamp(localX, 0.0f, width);
    localY = glm::clamp(localY, 0.0f, height);

    // Ray creation
    glm::vec3 ray = scene->camera.getMouseRay(
            localX,
            localY,
            static_cast<int>(width),
            static_cast<int>(height)
    );

    bool hitSomething = false;
    for (auto mesh : scene->meshes)
    {
        if (!mesh->is_visible)
            continue;

        if (mesh->rayIntersection(scene->camera.position, ray))
        {
            mesh->selected = !mesh->selected;
            hitSomething = true;
            break;
        }
    }

    if (!hitSomething) {
        std::cout << "Void Raycast\n";
        for (auto mesh : scene->meshes) {
            if (!mesh->selected)
                continue;

            mesh->applyAndUpdate(scene->modifier);
            mesh->selected = false;
        }
        scene->modifier.clear();
    }

#ifdef RAY_VISIBLE
    std::vector<float> rayVertices = generateRay(ray, scene->camera.position);
    raySize = rayVertices.size();
    glNamedBufferSubData(rayVBO, 0, raySize * sizeof(float), rayVertices.data());
#endif
}

void Ray::draw(unsigned int shaderProgram, Camera *camera) {
#ifdef RAY_VISIBLE
    glUseProgram(shaderProgram);
    Uniform::setBasicUniforms(shaderProgram, camera);

    glBindVertexArray(rayVAO);
    glDrawArrays(GL_LINES, 0, raySize / 6); // Warning : gridSize conversion from size_t to int
#endif
}

std::vector<float> Ray::generateRay(glm::vec3 rayDirection, glm::vec3 cameraPos) {
    std::vector<float> rayVertices;
    rayVertices.reserve(12);

    rayVertices.push_back(cameraPos.x + rayDirection.x * 100.0f);
    rayVertices.push_back(cameraPos.y + rayDirection.y * 100.0f);
    rayVertices.push_back(cameraPos.z + rayDirection.z * 100.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);

    rayVertices.push_back(cameraPos.x + rayDirection.x * -100.0f);
    rayVertices.push_back(cameraPos.y + rayDirection.y * -100.0f);
    rayVertices.push_back(cameraPos.z + rayDirection.z * -100.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);
    rayVertices.push_back(1.0f);

    return rayVertices;
}