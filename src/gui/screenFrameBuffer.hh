#pragma once

#include "glad/gl.h"
#include "backend/scene.hh"

class ScreenFrameBuffer {
private:
    GLuint screenFBO = 0;
    GLuint screenColorTex = 0;
    GLuint screenDepthTex = 0;
    float width = 0; // float is for resize operations
    float height = 0;

public:
    ScreenFrameBuffer()=default;

    void create(int width, int height);
    void load(Scene *scene);
    void resize();
    void bindTextures();
    void unbindTextures();

    GLuint getColorTexture() const { return screenColorTex; }
};