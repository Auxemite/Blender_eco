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
    public :
//        Texture() = default;
//        Texture(Texture&&) = default;
//        Texture& operator=(Texture&&) = default;
        Texture();
        Texture(int _width, int _height, const unsigned char* data);
        explicit Texture(const std::string& imagePath);
        Texture(const TextureData& data);
        Texture(const glm::uvec2 &size, ImageFormat format);
        ~Texture();

        const char* name() const;
        void setName(const std::string& name);

        void createTexture(const unsigned char* data);
        void bind(int unit) const;

        // Return number of mip levels needed
        int mipLevels() const;

    private:
        GLuint id = 0;
        int width;
        int height;
        ImageFormat format;
        std::string name_;
};