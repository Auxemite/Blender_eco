cmake_minimum_required(VERSION 3.22)
project(blender_eco)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(blender_eco main.cpp
        src/backend/utils/vector3.hh
        src/backend/utils/vector3.cpp
        src/backend/utils/color.hh
        src/backend/utils/color.cpp
        src/backend/image/image.hh
        src/backend/image/image.cpp
        src/backend/scene/object.hh
        src/backend/scene/object.cpp
        src/backend/scene/light.hh
        src/backend/scene/camera.hh
        src/backend/scene/camera.cpp
        src/backend/scene/scene.hh
        src/backend/scene/scene.cpp
        src/backend/scene/mesh.cpp
        src/backend/scene/mesh.hh
        src/backend/utils/texture_material.hh
        src/backend/utils/texture_material.cpp
        src/backend/utils/utils.hh
        src/backend/render/intersection.hh
        src/backend/render/intersection.cpp
)
