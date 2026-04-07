#include "editmodeRay.hh"
#include "utils/mathUtils.hh"

using namespace EditMode;

EditModeRay::EditModeRay(glm::vec3 cameraPos) : Ray(cameraPos) {
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

void EditModeRay::rayCasting(EditMode::EditModeScene& editModeScene, float width, float height) {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 imageMin = ImGui::GetItemRectMin();

    // Image position
    float localX = mousePos.x - imageMin.x;
    float localY = mousePos.y - imageMin.y;
    localX = glm::clamp(localX, 0.0f, width);
    localY = glm::clamp(localY, 0.0f, height);

    // Ray creation
    const Camera camera = editModeScene.camera();
    glm::vec3 rayDir = camera.getMouseRay(localX,localY,width,height);
    editModeScene.rayCasting(rayDir);
}