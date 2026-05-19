cmake_minimum_required(VERSION 3.22)
project(blender_eco)

set(CMAKE_CXX_STANDARD 20)
if(MSVC)
    set(COMPILE_OPTIONS /permissive- /W3 -D_CRT_SECURE_NO_DEPRECATE /Zc:preprocessor /wd5105)
else()
    set(EXTRA_WARNINGS -Wfloat-conversion)
    set(COMPILE_OPTIONS -pedantic -Werror -Wall -Wextra ${EXTRA_WARNINGS})
endif()

# external lib
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/Catch2)

include_directories(external/glfw/include)
include_directories(external/glad/include)
include_directories(external/imgui)
include_directories(external/glm)
include_directories(external)

include_directories(C:/Users/ernes/Desktop/Travail/Blender_eco/src)

file(GLOB_RECURSE SOURCE_FILES
        "src/*.hh"
        "src/*.cpp"
)

file(GLOB_RECURSE EXTERNAL_FILES
        "external/glad/*.c"
        "external/glad/*.h"
        "external/imgui/*.cpp"
        "external/imgui/*.h"
)

# Shader files
file(GLOB_RECURSE SHADER_FILES
        "shaders/*.frag"
        "shaders/*.vert"
        "shaders/*.geom"
        "shaders/*.tesc"
        "shaders/*.tese"
        "shaders/*.comp"
        "shaders/*.glsl"
)

add_executable(blender_eco
        ${SOURCE_FILES}
        ${EXTERNAL_FILES}
        ${SHADER_FILES}
        main.cpp
)

target_link_libraries(blender_eco glfw)
target_compile_options(blender_eco PUBLIC ${COMPILE_OPTIONS})

# =========================
# TESTS
# =========================

enable_testing()

file(GLOB_RECURSE TEST_FILES
        "tests/*.cpp"
        "tests/*.hh"
)

add_executable(tests
        ${SOURCE_FILES}
        ${EXTERNAL_FILES}
        ${SHADER_FILES}
        ${TEST_FILES}
)

target_link_libraries(tests PRIVATE
        Catch2::Catch2WithMain
        glfw
)