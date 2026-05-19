#pragma once

#include "graphics/imageFormat.hh"
#include <string>

struct TextureData {
    // TODO data;
    int width_;
    int height_;
    ImageFormat format_;
};

class Texture : NonMovable {
    public :
//        Texture() = default;
//        Texture(Texture&&) = default;
//        Texture& operator=(Texture&&) = default;
        Texture();
        Texture(int width, int height, const unsigned char* data);
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
        unsigned int id_ = 0;
        int width_;
        int height_;
        ImageFormat format_;
        std::string name_;
};