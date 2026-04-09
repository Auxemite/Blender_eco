#include "Catch2/src/catch2/catch_test_macros.hpp"
#include "scene/scene.hh"
#include "gui/window.hh"

TEST_CASE("Testsuite1") {
    GLFWwindow *window = Window::softwareContextInit();
    REQUIRE(window != nullptr);
    Window::shutDown(window);
}

TEST_CASE("Testsuite2") {
    GLFWwindow *window = Window::softwareContextInit();
    Scene scene = Scene();
    REQUIRE(scene.selectedMeshes().empty());
    Window::shutDown(window);
}

TEST_CASE("Testsuite3") {
    GLFWwindow *window = Window::softwareContextInit();
    Scene scene = Scene();
    REQUIRE(scene.selectedMeshes().empty());
    scene.addMesh("../data/cube.obj");
    REQUIRE(scene.selectedMeshes().empty());
    scene.addMesh("../data/bunny.obj");
    REQUIRE(scene.selectedMeshes().empty());
    scene.addMesh("../data/plane.obj");
    REQUIRE(scene.selectedMeshes().empty());
    scene.addTexture("../data/texture_test.jpg");
    REQUIRE(scene.selectedMeshes().empty());
    scene.linkTextureToMesh(0, 1);
    REQUIRE(scene.selectedMeshes().empty());
    scene.addMaterial();
    REQUIRE(scene.selectedMeshes().empty());
    scene.linkMaterialToMesh(0, 0);
    REQUIRE(scene.selectedMeshes().empty());
    Window::shutDown(window);
}