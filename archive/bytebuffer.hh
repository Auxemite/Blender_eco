#pragma once

#include <cstdint>
#include <cstddef>
#include "glad/gl.h"

namespace Graphics {

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using byte = std::byte;

    enum class BufferUsage {
        Attribute = GL_ARRAY_BUFFER,
        Index = GL_ELEMENT_ARRAY_BUFFER,
        Uniform = GL_UNIFORM_BUFFER,
        Storage = GL_SHADER_STORAGE_BUFFER,
    };

    class ByteBuffer {
    public:
        ByteBuffer() = default;
        ByteBuffer(ByteBuffer&&) = default; // move contructor
        ByteBuffer& operator=(ByteBuffer&&) = default; // move operator

        ByteBuffer(const void* data, size_t size);
        ~ByteBuffer();

        void bind(BufferUsage usage) const;
        void bind(BufferUsage usage, u32 index) const;

        u32 bufferId = 0;
        size_t size = 0;
        const void* data;
    };

}