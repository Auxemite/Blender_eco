#pragma once

#include "scene/scene.hh"
#include "graphics/ray.hh"
#include "editmode/editmodeRay.hh"

class ScreenFrameBuffer {
    public:
        ScreenFrameBuffer()=delete;
        ScreenFrameBuffer(int width, int height);
        void load(Scene& scene, Ray& ray);
        void loadEditMode(EditMode::EditModeScene& editModeScene, EditMode::EditModeRay& ray);
        void resize();
        void bindTextures();
        void unbindTextures();

    private:
        GLuint screenFBO_ = 0;
        GLuint screenColorTex_ = 0;
        GLuint screenDepthTex_ = 0;
        float width_ = 0; // float is for resize operations
        float height_ = 0;
};