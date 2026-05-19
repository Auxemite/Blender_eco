#pragma once

#include "graphics/utils.hh"

enum class ImageFormat {
    RGBA8_UNORM,
    RGBA8_sRGB,
    RGB8_UNORM,
    RGB8_sRGB,

    RGBA16_FLOAT,
    Depth32_FLOAT
};


struct ImageFormatGL {
    u32 format;
    u32 internalFormat;
    u32 componentType;
};

ImageFormatGL imageFormatToGL(ImageFormat format);