#include "Catch2/src/catch2/catch_test_macros.hpp"
#include "Catch2/src/catch2/catch_approx.hpp"

#include <filesystem>
#include <fstream>

#include "common/test_helpers.hh"
#include "graphics/grid.hh"
#include "graphics/imageFormat.hh"
#include "graphics/ray.hh"
#include "shader/shaderUtils.hh"

TEST_CASE("Image format conversions", "[graphics][image-format]") {
    const auto rgba8 = imageFormatToGL(ImageFormat::RGBA8_UNORM);
    REQUIRE(rgba8.format == GL_RGBA);

    const auto rgba8Srgb = imageFormatToGL(ImageFormat::RGBA8_sRGB);
    REQUIRE(rgba8Srgb.internalFormat == GL_SRGB8_ALPHA8);

    const auto rgb8 = imageFormatToGL(ImageFormat::RGB8_UNORM);
    REQUIRE(rgb8.internalFormat == GL_RGB8);

    const auto rgb8Srgb = imageFormatToGL(ImageFormat::RGB8_sRGB);
    REQUIRE(rgb8Srgb.internalFormat == GL_SRGB8);

    const auto rgba16f = imageFormatToGL(ImageFormat::RGBA16_FLOAT);
    REQUIRE(rgba16f.componentType == GL_FLOAT);

    const auto depth = imageFormatToGL(ImageFormat::Depth32_FLOAT);
    REQUIRE(depth.format == GL_DEPTH_COMPONENT);
}

TEST_CASE("Grid generation", "[graphics][grid]") {
    const std::vector<float> grid = generateGrid(2);
    REQUIRE_FALSE(grid.empty());
    REQUIRE(grid.size() == 144);
}

TEST_CASE("Ray generation", "[graphics][ray]") {
    Ray ray(glm::vec3(0.0f));
    const std::vector<float> vertices = ray.generateRay(glm::vec3(0.0f, 0.0f, 1.0f),
                                                        glm::vec3(0.0f, 0.0f, 0.0f));
    REQUIRE(vertices.size() == 12);
    REQUIRE(vertices[2] == Catch::Approx(100.0f));
    REQUIRE(vertices[8] == Catch::Approx(-100.0f));
}

TEST_CASE("Shader source read helper", "[utils][shader]") {
    const std::filesystem::path tempDir = std::filesystem::temp_directory_path() / "blender_eco_tests_read";
    std::filesystem::create_directories(tempDir);
    const std::filesystem::path srcFile = tempDir / "simple.vert";

    {
        std::ofstream out(srcFile);
        out << "#version 450 core\nvoid main(){}\n";
    }

    const std::string src = Shader::readShaderSource(srcFile.string());
    REQUIRE(src.find("#version 450 core") != std::string::npos);
}
