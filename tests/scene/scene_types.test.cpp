#include "Catch2/src/catch2/catch_test_macros.hpp"
#include "Catch2/src/catch2/catch_approx.hpp"

#include <cmath>

#include "common/test_helpers.hh"
#include "env.hh"
#include "gui/guiUtils.hh"
#include "scene/camera.hh"
#include "scene/light.hh"
#include "scene/material.hh"
#include "scene/modifier.hh"
#include "scene/triangle.hh"

namespace {

std::vector<glm::vec3*> makeTrianglePoints() {
    auto points = std::vector<glm::vec3*>();
    points.push_back(new glm::vec3(0.0f, 0.0f, 0.0f));
    points.push_back(new glm::vec3(1.0f, 0.0f, 0.0f));
    points.push_back(new glm::vec3(0.0f, 1.0f, 0.0f));
    return points;
}

void clearTrianglePoints(std::vector<glm::vec3*>& points) {
    for (glm::vec3* p : points) {
        delete p;
    }
    points.clear();
}

}

TEST_CASE("Triangle constructors and methods", "[scene][triangle]") {
    Triangle t1(glm::ivec3(0, 1, 2), glm::vec3(0.0f, 0.0f, 10.0f));
    REQUIRE(t1.vertexIndexes() == glm::ivec3(0, 1, 2));
    REQUIRE(glm::length(t1.normal()) == Catch::Approx(1.0f));

    auto points = makeTrianglePoints();
    Triangle t2(glm::ivec3(0, 1, 2), points);
    const float hit = t2.rayTriangleIntersection(points,
                                                 glm::vec3(0.25f, 0.25f, -1.0f),
                                                 glm::vec3(0.0f, 0.0f, 1.0f));
    REQUIRE(hit > 0.0f);

    t2.normalize();
    REQUIRE(glm::length(t2.normal()) == Catch::Approx(1.0f));
    clearTrianglePoints(points);
}

TEST_CASE("Modifier clear and state", "[scene][modifier]") {
    Modifier modifier;
    modifier.position = glm::vec3(1.0f, 2.0f, 3.0f);
    modifier.rotation = glm::vec3(1.0f);
    modifier.scale = 2.0f;
    REQUIRE_FALSE(modifier.isCleared());

    modifier.clear();
    REQUIRE(modifier.isCleared());
}

TEST_CASE("Material and light accessors", "[scene][material][scene][light]") {
    Material material(MATERIAL_TYPE::PBR);
    REQUIRE(material.name() != nullptr);
    REQUIRE(material.color().x == Catch::Approx(1.0f));
    REQUIRE(material.pbrFactor().x == Catch::Approx(0.0f));

    Light light(LightType::PointLight,
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(0.5f, 0.6f, 0.7f),
                42.0f);
    REQUIRE(light.position() == glm::vec3(1.0f, 2.0f, 3.0f));
    REQUIRE(light.color() == glm::vec3(0.5f, 0.6f, 0.7f));
    REQUIRE(light.intensity() == Catch::Approx(42.0f));
}

TEST_CASE("Camera getters and ray", "[scene][camera]") {
    Camera defaultCamera;
    REQUIRE(defaultCamera.position() == glm::vec3(0.0f));
    REQUIRE(defaultCamera.up() == glm::vec3(0.0f, 1.0f, 0.0f));

    Camera customCamera(glm::vec3(5.0f, 6.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    REQUIRE(customCamera.position() == glm::vec3(5.0f, 6.0f, 7.0f));

    Env::projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    const glm::vec3 ray = customCamera.getMouseRay(640.0f, 360.0f, 1280.0f, 720.0f);
    REQUIRE(glm::length(ray) == Catch::Approx(1.0f).margin(1e-3));
}

TEST_CASE("ImGui modulators are callable", "[scene][imgui]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    Gui::newFrame();

    Modifier modifier;
    modifier.positionModulator();
    modifier.rotationModulator();
    modifier.scaleModulator();

    Material material(MATERIAL_TYPE::PBR);
    material.colorModulator();
    material.pbrFactorModulator();

    Light light(LightType::PointLight, glm::vec3(0.0f), glm::vec3(1.0f), 1.0f);
    light.colorModulator();
    light.positionModulator();
    light.intensityModulator();

    Gui::render();
}

TEST_CASE("Camera input processing is callable", "[scene][camera][input]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    Camera camera;
    camera.processInputs(context.window(), 0.016f);

    const glm::vec3 position = camera.position();
    REQUIRE(std::isfinite(position.x));
    REQUIRE(std::isfinite(position.y));
    REQUIRE(std::isfinite(position.z));
}
