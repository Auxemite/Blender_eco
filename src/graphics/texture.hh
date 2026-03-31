#pragma once

#include "glm/glm.hpp"
#include "graphics/imageFormat.hh"
#include <string>

struct TextureData {
    // TODO data;
    int width;
    int height;
    ImageFormat format;
};

class Texture : NonMovable {
    private:
        GLuint id = 0;
        int width;
        int height;
        ImageFormat format;

    public :
//        Texture() = default;
//        Texture(Texture&&) = default;
//        Texture& operator=(Texture&&) = default;
        ~Texture();

        Texture();
        Texture(int _width, int _height, const unsigned char* data);
        explicit Texture(const std::string& imagePath);
        Texture(const TextureData& data);
        Texture(const glm::uvec2 &size, ImageFormat format);

        void createTexture(const unsigned char* data);
        void bind(int unit) const;

        // Return number of mip levels needed
        int mipLevels() const;
};