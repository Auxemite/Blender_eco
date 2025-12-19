#include "bytebuffer.hh"

namespace Graphics {

    ByteBuffer::ByteBuffer(const void* _data, size_t _size) {
        ALWAYS_ASSERT(_size, "Buffer size can not be 0");

//        GLuint _bufferId = 0;
//        glCreateBuffers(1, &_bufferId);
//
//        bufferId = _bufferId;
//        size = _size;
//
//        glNamedBufferData(bufferId, size, _data, GL_STATIC_DRAW);
//
//        data = _data;
    }

    ByteBuffer::~ByteBuffer() {
        if(bufferId) {
            glDeleteBuffers(1, &bufferId);
        }
    }

    void ByteBuffer::bind(BufferUsage usage) const {
        glBindBuffer(bufferUsageToGL(usage), bufferId);
    }

    void ByteBuffer::bind(BufferUsage usage, u32 index) const {
        ALWAYS_ASSERT(usage == BufferUsage::Uniform || usage == BufferUsage::Storage, "Index bind is only available for uniform and storage buffers");
        glBindBufferBase(bufferUsageToGL(usage), index, bufferId);
    }

}

