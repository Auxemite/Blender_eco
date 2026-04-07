#include <iostream>

#include "imgui/imgui.h"
#include "glad/gl.h"

#include "screenFrameBuffer.hh"
#include "graphics/graphics.hh"

ScreenFrameBuffer::ScreenFrameBuffer(int width, int height) {
    // Create main color texture
    screenColorTex_ = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &screenColorTex_);
    glTextureStorage2D(screenColorTex_, 1, GL_RGBA8, width, height);

    glTextureParameteri(screenColorTex_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(screenColorTex_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(screenColorTex_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(screenColorTex_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create main depth texture
    screenDepthTex_ = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &screenDepthTex_);
    glTextureStorage2D(screenDepthTex_, 1, GL_DEPTH24_STENCIL8, width, height);

    // Create and bind main FBO to main texture and depth texture
    screenFBO_ = 0;
    glCreateFramebuffers(1, &screenFBO_);
    glNamedFramebufferTexture(screenFBO_, GL_COLOR_ATTACHMENT0, screenColorTex_, 0);
    glNamedFramebufferTexture(screenFBO_, GL_DEPTH_STENCIL_ATTACHMENT, screenDepthTex_, 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glNamedFramebufferDrawBuffers(screenFBO_, 1, drawBuffers);

    if (glCheckNamedFramebufferStatus(screenFBO_, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "FBO not complete!" << std::endl;

    width_ = static_cast<float>(width);
    height_ = static_cast<float>(height);
}

void ScreenFrameBuffer::load(Scene& scene, Ray& ray) {
    ImGui::Begin("Texture Viewport");

    resize();

    ImGui::Image(
            (ImTextureID)(intptr_t)screenColorTex_,
            ImVec2(width_, height_),
            ImVec2(0, 1),
            ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        ray.rayCasting(scene, width_, height_);

    ImGui::End();
}

void ScreenFrameBuffer::loadEditMode(EditMode::EditModeScene& editModeScene, EditMode::EditModeRay& ray) {
    ImGui::Begin("Texture Viewport");

    resize();

    ImGui::Image(
            (ImTextureID)(intptr_t)screenColorTex_,
            ImVec2(width_, height_),
            ImVec2(0, 1),
            ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        ray.rayCasting(editModeScene, width_, height_);
    }

    ImGui::End();
}

void ScreenFrameBuffer::resize() {
    const float targetRatio = 16.0f / 9.0f;

    ImVec2 avail = ImGui::GetContentRegionAvail();

    width_  = avail.x;
    height_ = width_ / targetRatio;

    if (height_ > avail.y)
    {
        height_ = avail.y;
        width_  = height_ * targetRatio;
        std::cout << "Resize screen viewport to " << width_ << " x " << height_ << "\n";
    }

    float offsetX = (avail.x - width_) * 0.5f;
    float offsetY = (avail.y - height_) * 0.5f;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
}

void ScreenFrameBuffer::bindTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO_);
    glViewport(0, 0, WIDTH, HEIGHT);

    Graphics::clearFrameBuffer();
}

void ScreenFrameBuffer::unbindTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Graphics::clearFrameBuffer();
}
