#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "screenFrameBuffer.hh"
#include "glm/vec3.hpp"
#include "glm/common.hpp"
#include "graphics/graphics.hh"

ScreenFrameBuffer::ScreenFrameBuffer(int _width, int _height) {
    width = _width;
    height = _height;
    // Create main color texture
    screenColorTex = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &screenColorTex);
    glTextureStorage2D(screenColorTex, 1, GL_RGBA8, width, height);

    glTextureParameteri(screenColorTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(screenColorTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(screenColorTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(screenColorTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create main depth texture
    screenDepthTex = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &screenDepthTex);
    glTextureStorage2D(screenDepthTex, 1, GL_DEPTH24_STENCIL8, width, height);

    // Create and bind main FBO to main texture and depth texture
    screenFBO = 0;
    glCreateFramebuffers(1, &screenFBO);
    glNamedFramebufferTexture(screenFBO, GL_COLOR_ATTACHMENT0, screenColorTex, 0);
    glNamedFramebufferTexture(screenFBO, GL_DEPTH_STENCIL_ATTACHMENT, screenDepthTex, 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glNamedFramebufferDrawBuffers(screenFBO, 1, drawBuffers);

    if (glCheckNamedFramebufferStatus(screenFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "FBO not complete!" << std::endl;
}

void ScreenFrameBuffer::load(Scene *scene) {
    ImGui::Begin("Texture Viewport");

    resize();

    ImGui::Image(
            (ImTextureID)(intptr_t)screenColorTex,
            ImVec2(width, height),
            ImVec2(0, 1),
            ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
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
                mesh->selected = false;
                if (!mesh->is_visible)
                    continue;

                mesh->applyAndUpdate(scene->modifier);
            }
            scene->modifier.clear();
        }
    }

    ImGui::End();
}

void ScreenFrameBuffer::resize() {
    const float targetRatio = 16.0f / 9.0f;

    ImVec2 avail = ImGui::GetContentRegionAvail();

    width  = avail.x;
    height = width / targetRatio;

    if (height > avail.y)
    {
        height = avail.y;
        width  = height * targetRatio;
        std::cout << "Resize screen viewport to " << width << " x " << height << "\n";
    }

    float offsetX = (avail.x - width) * 0.5f;
    float offsetY = (avail.y - height) * 0.5f;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
}

void ScreenFrameBuffer::bindTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
    glViewport(0, 0, WIDTH, HEIGHT);

    Graphics::clearFrameBuffer();
}

void ScreenFrameBuffer::unbindTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Graphics::clearFrameBuffer();
}
