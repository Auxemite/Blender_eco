#pragma once

#include "bytebuffer.hh"

namespace Graphics {
    template<typename T>
    class TypedBuffer : public ByteBuffer {
    public:
        TypedBuffer() = default;

        TypedBuffer(Span<const T> data) : TypedBuffer(data.data(), data.size()) {}

        TypedBuffer(const T* data, size_t count) : ByteBuffer(data, count * sizeof(T)) {}

        size_t elementCount() const {
//            DEBUG_ASSERT(size % sizeof(T) == 0);
            return size / sizeof(T);
        };
    };
}