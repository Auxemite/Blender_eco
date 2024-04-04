//
// Created by ernes on 02/04/2024.
//

#include <iostream>
#include <vector>
#include "app.h"

class Color {
public:
    double r;
    double g;
    double b;
};

namespace app {
    void RenderUI()
    {
        ImGui::Begin("Tutuuuuut");
        ImGui::Button("Hello");
        static float value = 0.0f;
        ImGui::DragFloat("Value", &value);
        ImGui::End();
//        ImDrawList::AddImage();
        ImGui::ShowDemoWindow();
    }

    void convertDataToGLRGB(const std::vector<std::vector<Color>>& data, int width, int height, unsigned char* buffer) {
        int index = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                buffer[index++] = static_cast<unsigned char>(data[y][x].r * 255);
                buffer[index++] = static_cast<unsigned char>(data[y][x].g * 255);
                buffer[index++] = static_cast<unsigned char>(data[y][x].b * 255);
            }
        }
    }

    void LoadTexture(const char* filename)
    {
        int width, height;
        unsigned char* data = load(filename);
        std::vector<unsigned char> buffer(width * height * 3);
        convertDataToGLRGB(data, width, height, buffer.data());
        if (data == nullptr)
        {
            std::cout << "Failed to load texture %s\n" << filename;
            return;
        }
        ImGui::GetTexture
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}
