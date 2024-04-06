cmake_minimum_required(VERSION 3.27)
project(blender_eco)

set(CMAKE_CXX_STANDARD 17)

add_executable(blender_eco main.cpp
        src/frontend/imgui/imconfig.h
        src/frontend/imgui/imgui.cpp
        src/frontend/imgui/imgui.h
        src/frontend/imgui/imgui_demo.cpp
        src/frontend/imgui/imgui_draw.cpp
        src/frontend/imgui/imgui_internal.h
        src/frontend/imgui/imgui_tables.cpp
        src/frontend/imgui/imgui_widgets.cpp
        src/frontend/imgui/imstb_rectpack.h
        src/frontend/imgui/imstb_textedit.h
        src/frontend/imgui/imstb_truetype.h
        src/frontend/imgui/imgui_impl_glfw.cpp
        src/frontend/imgui/imgui_impl_opengl3.cpp
        src/frontend/imgui/imgui_impl_glfw.h
        src/frontend/imgui/imgui_impl_opengl3.h
        src/frontend/imgui/imgui_impl_opengl3_loader.h

        src/frontend/app.cpp
        src/frontend/app.h

        src/backend/src/vector3.hh
        src/backend/src/color.hh
        src/backend/src/image.hh
        src/backend/src/object/object.hh
        src/backend/src/light.hh
        src/backend/src/camera.hh
        src/backend/src/scene.hh
        src/backend/src/texture_material.hh
        src/backend/src/utils.hh
        src/backend/src/intersection.hh
        src/backend/src/intersection.cpp
        src/backend/render.hh
        src/backend/src/image.cpp
)

find_package(OpenGL REQUIRED)
target_link_libraries(blender_eco PUBLIC OpenGL::GL)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(blender_eco PUBLIC glfw)