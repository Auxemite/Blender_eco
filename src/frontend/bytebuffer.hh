#pragma once

#include "graphics.hh"

namespace Graphics {

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