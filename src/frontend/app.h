#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "../backend/src/image.hh"
#include "GLFW/glfw3.h"

namespace app {
    void RenderUI();
    void Windows();
    void convertDataToGLRGB(const vector<vector<Color>>& data, int w, int h, unsigned char* buffer);
    bool LoadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
}