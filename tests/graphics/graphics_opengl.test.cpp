#include "Catch2/src/catch2/catch_test_macros.hpp"

#include <filesystem>

#include "common/test_helpers.hh"
#include "env.hh"
#include "graphics/graphics.hh"
#include "graphics/graphicsObject.hh"
#include "graphics/texture.hh"
#include "graphics/uniform.hh"
#include "scene/light.hh"
#include "scene/mesh.hh"
#include "scene/modifier.hh"
#include "utils/shaderUtils.hh"

namespace {

std::vector<Engine::vertex> testVertices() {
    return {
            {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(), glm::vec2(), glm::vec4(), glm::vec3(1.0f, 0.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(), glm::vec2(), glm::vec4(), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(), glm::vec2(), glm::vec4(), glm::vec3(0.0f, 0.0f, 1.0f)},
    };
}

}

TEST_CASE("Texture and GraphicsObject operations", "[graphics][opengl]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    const unsigned char pixels[] = {
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255,
            255, 255, 255, 255
    };

    Texture texture(2, 2, pixels);
    REQUIRE(texture.name() != nullptr);
    texture.setName("renamed_texture");
    REQUIRE(std::string(texture.name()) == "renamed_texture");
    REQUIRE(texture.mipLevels() == 2);
    texture.bind(0);

    GraphicsObject go(testVertices());
    REQUIRE_FALSE(go.isLinkedTexture(&texture));
    go.linkToGraphicObject(&texture);
    REQUIRE(go.isLinkedTexture(&texture));
    go.updateVBOFromMesh(testVertices());
    go.draw();

    texture.createTexture(pixels);
}

TEST_CASE("Uniform and Graphics namespace functions", "[graphics][uniform][graphics]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    const unsigned int program = TestHelpers::createMinimalProgram();
    REQUIRE(program != 0);
    glUseProgram(program);

    const Mesh mesh(TestHelpers::dataFile("cube.obj"));
    const Camera camera;
    Modifier modifier;
    Light light(LightType::PointLight, glm::vec3(1.0f), glm::vec3(1.0f), 2.0f);

    Uniform::setModelViewProj(program, camera);
    Uniform::setBasicUniforms(program, camera);
    Uniform::setModifierUniforms(program, modifier);
    Uniform::setMeshUniforms(program, mesh);
    Uniform::setMaterialAndTextureUniforms(program, mesh);
    Uniform::setLightUniforms(program, light);
    Uniform::setUniqueColorUniforms(program, glm::vec3(1.0f, 0.0f, 0.0f));

    Graphics::clearFrameBuffer();
    Graphics::checkOpenGLError("unit-test");
    Graphics::audit_bindings();

    Scene scene;
    Graphics::drawInterfaceObject(program, scene);

    glDeleteProgram(program);
}

TEST_CASE("Shader OpenGL workflow", "[utils][shader][opengl]") {
    TestHelpers::ScopedWindowContext context;
    REQUIRE(context.valid());

    const std::string vertexSource =
            "#version 450 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "void main(){ gl_Position = vec4(aPos, 1.0); }\n";

    const std::string fragmentSource =
            "#version 450 core\n"
            "out vec4 FragColor;\n"
            "void main(){ FragColor = vec4(1.0); }\n";

    const unsigned int vs = Shader::compileShader(vertexSource, GL_VERTEX_SHADER, "Vertex");
    const unsigned int fs = Shader::compileShader(fragmentSource, GL_FRAGMENT_SHADER, "Fragment");
    REQUIRE(vs != 0);
    REQUIRE(fs != 0);

    const std::filesystem::path shaderDir = std::filesystem::temp_directory_path() / "blender_eco_shader_tests";
    const std::filesystem::path vsPath = TestHelpers::createTempShaderFile(shaderDir, "test.vert", vertexSource);
    const std::filesystem::path fsPath = TestHelpers::createTempShaderFile(shaderDir, "test.frag", fragmentSource);

    const unsigned int initProgram = Shader::initShaderProgram(vsPath.string(), fsPath.string());
    REQUIRE(initProgram != 0);

    const unsigned int programFromDir = Shader::createShaderProgram(shaderDir.string());
    REQUIRE(programFromDir != 0);

    const unsigned int invalidProgram = Shader::createShaderProgram((shaderDir / "missing").string());
    REQUIRE(invalidProgram == 0);

    Shader::checkOpenGLError("shader-utils-test");

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(initProgram);
    glDeleteProgram(programFromDir);
}
