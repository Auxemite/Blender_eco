#include <iostream>
#include "texture.hh"
#include "imageFormat.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

Texture::Texture(int _width, int _height, const unsigned char* data) {
    width = _width;
    height = _height;

    format = ImageFormat::RGB8_UNORM;

    createTexture(data);
}

Texture::Texture(const std::string& imagePath) {
    stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &texWidth, &texHeight, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
    }

    width = texWidth;
    height = texHeight;

    // TODO verify image format based on channels
    // format = ImageFormat::RGBA8_UNORM; // Does the "photo of a screen" effect for whatever reason
    format = ImageFormat::RGBA8_UNORM;

    createTexture(data);
    stbi_image_free(data);
}

void Texture::createTexture(const unsigned char* data) {
    ImageFormatGL formatGl = imageFormatToGL(this->format);
    // Texture Creation
    glCreateTextures(GL_TEXTURE_2D, 1, &id);

    // Texture Allocation
    glTextureStorage2D(id, mipLevels(), formatGl.internalFormat, width, height);

    // For alignement if image size is not a multiple of 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Texture Upload Data
    glTextureSubImage2D(id, 0, 0, 0, width, height,
                        formatGl.format, formatGl.componentType, data);

    // Texture Parameters
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Mipmap Generation
    glGenerateTextureMipmap(id);
}

void Texture::bind(int unit) const {
    glBindTextureUnit(unit, id);
}

// Return number of mip levels needed
int Texture::mipLevels() const {
    const float side = static_cast<float>(std::max(width, height));
    return 1 + static_cast<int>(std::floor(std::log2(side)));
}