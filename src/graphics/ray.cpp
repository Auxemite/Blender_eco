#include <vector>

#include "glm/vec3.hpp"
#include "imgui/imgui.h"

#include "ray.hh"
#include "uniform.hh"
#include "utils/mathUtils.hh"

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

void Ray::rayCasting(Scene *scene, EditModeScene *editModeScene, float width, float height) {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 imageMin = ImGui::GetItemRectMin();

    // Image position
    float localX = mousePos.x - imageMin.x;
    float localY = mousePos.y - imageMin.y;
    localX = glm::clamp(localX, 0.0f, width);
    localY = glm::clamp(localY, 0.0f, height);

    // Ray creation
    if (Env::editmode)
        ray = editModeScene->camera_.getMouseRay(localX,localY,width,height);
    else
        ray = scene->camera.getMouseRay(localX,localY,width,height);

    if (!Env::editmode)
        hitMeshTest(scene);
    else if (editModeScene->editmodeType_ == FACE)
        hitMeshFaceTest(editModeScene);
//    else if (editModeScene->editmodeType_ == EDGE)
//        hitMeshEdgeTest(editModeScene);
    else if (editModeScene->editmodeType_ == VERTEX)
        hitMeshVertexTest(editModeScene, 0.2f);

#ifdef RAY_VISIBLE
    std::vector<float> rayVertices = generateRay(ray, scene->camera.position);
    raySize = rayVertices.size();
    glNamedBufferSubData(rayVBO, 0, raySize * sizeof(float), rayVertices.data());
#endif
}

void Ray::hitMeshTest(Scene *scene) {
    float closestHitDistance = 0.0f;
    Mesh *hitMesh = nullptr;
    for (auto mesh : scene->meshes) {
        if (!mesh->is_visible)
            continue;

        if (mesh->selected)
            mesh->applyAndUpdate(scene->modifier);

        mesh->selected = false;
        float hitDistance = mesh->rayIntersection(scene->camera.position, ray);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance)) {
            closestHitDistance = hitDistance;
            hitMesh = mesh;
        }
    }

    scene->clearSelectedMeshList();
    if (hitMesh) {
        hitMesh->selected = true;
        scene->selectedMeshes.push_back(hitMesh);
        std::cout << "Hit Mesh\n";
    }
    else
        std::cout << "Void Raycast\n";

    scene->modifier.clear();
}

void Ray::hitMeshFaceTest(EditModeScene *scene) {
    if (!Env::editmode || scene->selectedMeshes_.empty())
        return;

    Mesh *mesh = scene->selectedMeshes_[0];
    if (!mesh->selected)
        std::cerr << "HitMeshFaceTest Warning : Mesh should be selected\n";

    mesh->applySelectedAndUpdate(scene->modifier_);
    if (!scene->shiftMode_)
        mesh->selectedPoints.clear();

    float closestHitDistance = 0;
    Triangle *hitFace = nullptr;
    for (auto face : mesh->faces) {
        float hitDistance = face->rayIntersection(mesh->points, scene->camera_.position, ray);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance)) {
            closestHitDistance = hitDistance;
            hitFace = face;
        }
    }

    if (hitFace) {
        std::cout << "Face touched " << hitFace->ia << " " << hitFace->ib << " " << hitFace->ic << "\n";
        if (scene->shiftMode_
            && mesh->selectedPoints.contains(hitFace->ia)
            && mesh->selectedPoints.contains(hitFace->ib)
            && mesh->selectedPoints.contains(hitFace->ic))
        {
            mesh->selectedPoints.erase(hitFace->ia);
            mesh->selectedPoints.erase(hitFace->ib);
            mesh->selectedPoints.erase(hitFace->ic);
            mesh->graphicsObject->updateVBOFromMesh(mesh->verticesEditmode());
        }
        else {
            mesh->selectedPoints.insert(hitFace->ia);
            mesh->selectedPoints.insert(hitFace->ib);
            mesh->selectedPoints.insert(hitFace->ic);
            mesh->graphicsObject->updateVBOFromMesh(mesh->verticesEditmode());
        }
    }
    else
        std::cout << "Void Raycast\n";

    scene->modifier_.clear();
}

void Ray::hitMeshEdgeTest(EditModeScene *scene) {
    std::cerr << "HitMeshEdgeTest Warning : Not Implemented\n";
}

void Ray::hitMeshVertexTest(EditModeScene *scene, float radius) {
    if (!Env::editmode || scene->selectedMeshes_.empty())
        return;

    Mesh *mesh = scene->selectedMeshes_[0];
    if (!mesh->selected)
        std::cerr << "HitMeshFaceTest Warning : Mesh should be selected\n";

    mesh->applySelectedAndUpdate(scene->modifier_);
    if (!scene->shiftMode_)
        mesh->selectedPoints.clear();

    int hitVertexIndex = -1;
    float closestHitDistance = 0;
    for (int i = 0; i < mesh->points.size(); i++) {
        float hitDistance = sphereIntersection(scene->camera_.position, *mesh->points[i], radius);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance)) {
            closestHitDistance = hitDistance;
            hitVertexIndex = i;
        }
    }

    if (hitVertexIndex != -1) {
        std::cout << "Vertex touched " << hitVertexIndex << "\n";
        if (mesh->selectedPoints.contains(hitVertexIndex)) {
            mesh->selectedPoints.erase(hitVertexIndex);
            mesh->graphicsObject->updateVBOFromMesh(mesh->verticesEditmode());
        }
        else {
            mesh->selectedPoints.insert(hitVertexIndex);
            mesh->graphicsObject->updateVBOFromMesh(mesh->verticesEditmode());
        }
    }
    else
        std::cout << "Void Raycast\n";

    scene->modifier_.clear();
}

float Ray::sphereIntersection(glm::vec3 cameraPos, glm::vec3 center, float radius) {
    glm::vec3 oc = cameraPos - center;

    float a = dot(ray, ray);
    float b = 2.0f * dot(oc, ray);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = discr(a, b, c);

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0f * a);
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
