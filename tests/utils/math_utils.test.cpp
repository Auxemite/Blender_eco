#include "Catch2/src/catch2/catch_test_macros.hpp"
#include "Catch2/src/catch2/catch_approx.hpp"

#include "utils/mathUtils.hh"

namespace {

bool approxVec3(const glm::vec3& a, const glm::vec3& b, float eps = 1e-4f) {
    return glm::length(a - b) < eps;
}

}

TEST_CASE("Math scalar helpers", "[utils][math]") {
    REQUIRE(Math::sqr(3.0f) == Catch::Approx(9.0f));
    REQUIRE(Math::discr(1.0f, 2.0f, 1.0f) == Catch::Approx(0.0f));
    REQUIRE(Math::abs(-12.5f) == Catch::Approx(12.5f));
}

TEST_CASE("Math rotation matrix", "[utils][math]") {
    const glm::mat3 identity = Math::getRotationMatrix(glm::vec3(0.0f));
    const glm::vec3 rotated = identity * glm::vec3(2.0f, -3.0f, 4.0f);
    REQUIRE(approxVec3(rotated, glm::vec3(2.0f, -3.0f, 4.0f)));
}

TEST_CASE("Math ray sphere intersection", "[utils][math]") {
    const float hit = Math::raySphereIntersection(glm::vec3(0.0f, 0.0f, -5.0f),
                                                  glm::vec3(0.0f, 0.0f, 1.0f),
                                                  glm::vec3(0.0f),
                                                  1.0f);
    REQUIRE(hit > 0.0f);

    const float miss = Math::raySphereIntersection(glm::vec3(0.0f, 0.0f, -5.0f),
                                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec3(0.0f),
                                                   1.0f);
    REQUIRE(miss < 0.0f);
}

TEST_CASE("Math ray triangle intersection", "[utils][math]") {
    const glm::vec3 a(-1.0f, -1.0f, 0.0f);
    const glm::vec3 b(1.0f, -1.0f, 0.0f);
    const glm::vec3 c(0.0f, 1.0f, 0.0f);

    const float hit = Math::rayTriangleIntersection(glm::vec3(0.0f, 0.0f, -2.0f),
                                                    glm::vec3(0.0f, 0.0f, 1.0f),
                                                    a, b, c);
    REQUIRE(hit > 0.0f);

    const float miss = Math::rayTriangleIntersection(glm::vec3(0.0f, 0.0f, -2.0f),
                                                     glm::vec3(0.0f, 1.0f, 0.0f),
                                                     a, b, c);
    REQUIRE(miss == Catch::Approx(0.0f));
}

TEST_CASE("Math closest edge", "[utils][math]") {
    const glm::ivec2 edge = Math::getClosestEdge(glm::vec3(0.8f, 0.0f, 0.0f),
                                                 glm::vec3(1.0f, 0.0f, 0.0f),
                                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                                 glm::vec3(0.0f, 0.0f, 1.0f),
                                                 10, 11, 12);

    REQUIRE((edge.x == 10 || edge.x == 12));
    REQUIRE((edge.y == 11 || edge.y == 12));
}
