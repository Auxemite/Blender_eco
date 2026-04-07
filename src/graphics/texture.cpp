#include <iostream>
#include "texture.hh"
#include "imageFormat.hh"
#include "glm/glm.hpp"
#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

static int textureNumber = 0;

Texture::Texture() {
    unsigned char data[] = {255, 255, 255, 255};
    width_ = 1;
    height_ = 1;
    name_ = "texture_" + std::to_string(textureNumber++);
    format_ = ImageFormat::RGBA8_UNORM;

    createTexture(data);
}

Texture::Texture(int width, int height, const unsigned char* data) {
    width_ = width;
    height_ = height;
    name_ = "texture_" + std::to_string(textureNumber++);
    format_ = ImageFormat::RGBA8_UNORM;

    createTexture(data);
}

Texture::Texture(const std::string& imagePath) {
    stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &texWidth, &texHeight, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
    }

    width_ = texWidth;
    height_ = texHeight;
    name_ = "texture_" + std::to_string(textureNumber++);

    // TODO verify image format based on channels
    // format = ImageFormat::RGBA8_UNORM; // Does the "photo of a screen" effect for whatever reason
    format_ = ImageFormat::RGBA8_UNORM;

    createTexture(data);
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

const char* Texture::name() const {
    return name_.c_str();
}

void Texture::setName(const std::string& name) {
    name_ = name;
}

void Texture::createTexture(const unsigned char* data) {
    ImageFormatGL formatGl = imageFormatToGL(this->format_);
    // Texture Creation
    glCreateTextures(GL_TEXTURE_2D, 1, &id_);

    // Texture Allocation
    glTextureStorage2D(id_, mipLevels(), formatGl.internalFormat, width_, height_);

    // For alignement if image size is not a multiple of 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Texture Upload Data
    glTextureSubImage2D(id_, 0, 0, 0, width_, height_,
                        formatGl.format, formatGl.componentType, data);

    // Texture Parameters
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Mipmap Generation
    glGenerateTextureMipmap(id_);
}

void Texture::bind(int unit) const {
    glBindTextureUnit(unit, id_);
}

// Return number of mip levels needed
int Texture::mipLevels() const {
    const float side = static_cast<float>(std::max(width_, height_));
    return 1 + static_cast<int>(std::floor(std::log2(side)));
}