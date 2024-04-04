cmake_minimum_required(VERSION 3.27)
project(blender_eco)

set(CMAKE_CXX_STANDARD 17)
#add_link_options(-lopengl32 -lGL -lglu32 -lglut -lmingw32)

add_executable(blender_eco main.cpp
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3_loader.h
        app.cpp
        app.h
        test.h
)

find_package(OpenGL REQUIRED)
target_link_libraries(blender_eco PUBLIC OpenGL::GL)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(blender_eco PUBLIC glfw)