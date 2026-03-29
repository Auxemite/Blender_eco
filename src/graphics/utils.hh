#pragma once

#include "defines.hh"
#include "env.hh"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using byte = std::byte;

//// Print message and terminate immediatly
#define FATAL(msg) Graphics::fatal((msg), __FILE__, __LINE__)
//// Assert in debug and release
#define ALWAYS_ASSERT(cond, msg) do { if(!(cond)) { FATAL(msg); } } while(false)

#ifdef NDEBUG
#else
#define BLENDER_ECO_DEBUG
#define DEBUG_ASSERT(cond) ALWAYS_ASSERT(cond, "Assert failed: " #cond)
#endif

struct NonCopyable {
    inline constexpr NonCopyable() {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

struct NonMovable : NonCopyable {
    inline constexpr NonMovable() {}
    NonMovable(const NonMovable&) = delete;
    NonMovable& operator=(const NonMovable&) = delete;

    NonMovable(NonMovable&&) = delete;
};

namespace Graphics {


#ifdef OS_WIN
bool running_in_debugger();
#endif

void break_in_debugger();
void fatal(const char* msg, const char* file = nullptr, int line = 0);

enum class AccessType {
    WriteOnly,
    ReadOnly,
    ReadWrite
};

template<typename T>
class Span {
    template<typename U>
    static constexpr bool is_compat = std::is_constructible_v<T*, U>;

    template<typename C>
    using data_type = decltype(std::declval<C>().data());

    public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

    inline constexpr Span() = default;
    inline constexpr Span(const Span&) = default;
    inline constexpr Span& operator=(const Span&) = default;

    inline constexpr Span(std::nullptr_t) {}
    inline constexpr Span(T& t) : _data(&t), _size(1) {}
    inline constexpr Span(T* data, size_t size) : _data(data), _size(size) {}

    template<size_t N>
    inline constexpr Span(T (&arr)[N]) : _data(arr), _size(N) {}

    template<size_t N>
    inline constexpr Span(std::array<T, N>& arr) : _data(arr.data()), _size(N) {}

    template<typename C, typename = std::enable_if_t<is_compat<data_type<C>>>>
    inline constexpr Span(C&& vec) : _data(vec.data()), _size(std::distance(vec.begin(), vec.end())) {}

    inline constexpr size_t size() const { return _size; }
    inline constexpr bool is_empty() const { return !_size; }
    inline constexpr T* data() { return _data; }
    inline constexpr const T* data() const { return _data; }
    inline constexpr const_iterator begin() const { return _data; }
    inline constexpr const_iterator end() const { return _data + _size; }

    bool operator==(const Span& other) const { return size() == other.size() && std::equal(begin(), end(), other.begin(), other.end()); }
    bool operator!=(const Span& other) const { return !operator==(other); }

    inline constexpr T& operator[](size_t i) const {
        DEBUG_ASSERT(i < size());
        return _data[i];
    }

    private:
    T* _data = nullptr;
    size_t _size = 0;

};
}