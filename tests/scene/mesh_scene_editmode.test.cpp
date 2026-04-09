#include "Catch2/src/catch2/catch_test_macros.hpp"

#include <cmath>

#include "common/test_helpers.hh"
#include "editmode/editmodeRay.hh"
#include "editmode/editmodeScene.hh"
#include "env.hh"
#include "graphics/grid.hh"
#include "gui/guiUtils.hh"
#include "scene/material.hh"
#include "scene/mesh.hh"
#include "scene/scene.hh"

TEST_CASE("Mesh functionality", "[scene][mesh]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    const std::string cubePath = TestHelpers::dataFile("cube.obj");
    Mesh mesh(cubePath);
    REQUIRE(mesh.name() != nullptr);

    REQUIRE_FALSE(mesh.isSelected());
    mesh.setSelection(true);
    REQUIRE(mesh.isSelected());

    REQUIRE(mesh.isVisible());
    mesh.setVisibility(false);
    REQUIRE_FALSE(mesh.isVisible());
    mesh.setVisibility(true);

    Material mat(MATERIAL_TYPE::PBR);
    mesh.linkToMesh(&mat);
    REQUIRE(mesh.isLinkedMaterial(&mat));

    Texture tex;
    mesh.linkToMesh(&tex);
    REQUIRE(mesh.isLinkedTexture(&tex));

    const glm::vec3 mean = mesh.vertexMean();
    REQUIRE(std::isfinite(mean.x));

    const auto rayInfo = mesh.rayMeshIntersection(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    REQUIRE(rayInfo.hitDistance >= 0.0f);

    const int vertexHit = mesh.rayMeshIntersection(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.5f);
    REQUIRE(vertexHit >= -1);

    mesh.update();

    Modifier modifier;
    modifier.position = glm::vec3(0.1f, 0.0f, 0.0f);
    mesh.applyAndUpdate(modifier);

    mesh.addToSelectedPoints(0);
    modifier.position = glm::vec3(0.0f, 0.1f, 0.0f);
    mesh.applySelectedAndUpdate(modifier);
    mesh.applyEditModeSelection();

    auto verts = mesh.vertices();
    REQUIRE_FALSE(verts.empty());

    auto editVerts = mesh.verticesEditmode();
    REQUIRE(editVerts.size() == verts.size());

    Env::shiftMode = false;
    mesh.addToSelectedPoints(1);
    Env::shiftMode = true;
    mesh.addToSelectedPoints(1);
    mesh.clearSelectedPoints();

    mesh.draw();
}

TEST_CASE("Scene functionality", "[scene][core]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    Scene scene;
    REQUIRE(scene.selectedMeshes().empty());

    scene.addMesh(TestHelpers::dataFile("cube.obj"));
    scene.addMesh(TestHelpers::dataFile("plane.obj"));
    scene.duplicateMesh(0);
    scene.deleteMesh(1);

    scene.addMaterial();
    scene.linkMaterialToMesh(0, 0);
    scene.linkMaterialToSelectedMesh(0, 0);
    scene.deleteMaterial(0);

    scene.addTexture(TestHelpers::dataFile("texture_test.jpg"));
    scene.linkTextureToMesh(0, 0);
    scene.linkTextureToSelectedMesh(0, 0);
    scene.deleteTexture(0);

    scene.clearSelectedMeshList();
    scene.rayCasting(glm::vec3(0.0f, 0.0f, 1.0f));

    scene.processInputs(context.window(), 0.016f);

    const unsigned int program = TestHelpers::createMinimalProgram();
    glUseProgram(program);

    scene.applyAndUpdate();
    scene.drawMeshes(program, glm::vec3(0.2f));
    scene.drawSelectedMeshes(program, glm::vec3(1.0f, 1.0f, 0.0f));
    scene.drawOutline(program);

    glDeleteProgram(program);
}

TEST_CASE("Edit mode scene and ray", "[editmode]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    Scene scene;
    scene.addMesh(TestHelpers::dataFile("cube.obj"));
    scene.rayCasting(glm::vec3(0.0f, 0.0f, 1.0f));

    EditMode::EditModeScene editModeScene(scene);
    editModeScene.updateSceneCenter();
    editModeScene.synchronizeScene(scene);

    Env::editmode = true;
    Env::shiftMode = false;

    editModeScene.applyAndUpdate();

    const unsigned int program = TestHelpers::createMinimalProgram();
    glUseProgram(program);
    editModeScene.drawSelectedMeshes(program, glm::vec3(1.0f));

    editModeScene.rayCasting(glm::vec3(0.0f, 0.0f, 1.0f));
    editModeScene.processInputs(context.window(), 0.016f);

    Gui::newFrame();
    editModeScene.modifierModulator();
    editModeScene.editModeModulator(scene);
    Gui::render();

    EditMode::toggleEditMode(scene, editModeScene);

    EditMode::EditModeRay editRay(glm::vec3(0.0f, 0.0f, 5.0f));
    Gui::newFrame();
    ImGui::Begin("RayCastHost");
    ImGui::Text("Ray host");
    editRay.rayCasting(editModeScene, 100.0f, 100.0f);
    ImGui::End();
    Gui::render();

    glDeleteProgram(program);
}

TEST_CASE("VisualGrid draw path", "[graphics][grid][opengl]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    const unsigned int program = TestHelpers::createMinimalProgram();
    glUseProgram(program);

    VisualGrid grid;
    Camera camera;
    grid.draw(program, camera);

    glDeleteProgram(program);
}

TEST_CASE("Ray scene casting and draw", "[graphics][ray][opengl]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    Scene scene;
    scene.addMesh(TestHelpers::dataFile("cube.obj"));

    Ray ray(glm::vec3(0.0f, 0.0f, 5.0f));

    Gui::newFrame();
    ImGui::Begin("RayHost");
    ImGui::Text("Ray host");
    ray.rayCasting(scene, 100.0f, 100.0f);
    ImGui::End();
    Gui::render();

    const unsigned int program = TestHelpers::createMinimalProgram();
    glUseProgram(program);
    ray.draw(program, scene.camera());

    glDeleteProgram(program);
}
