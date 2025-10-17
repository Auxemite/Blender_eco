<a id="readme-top"></a>
# 3D Modeling Software

<br />
<div align="center">
  <a href="https://github.com/Auxemite/Blender_eco/">
    <img src="data/interface_screeshot.webp" alt="3D Modeling Software"> <!-- width="80" height="80"> -->
  </a>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

The goal of this project was to create 3D modeling software based on how Blender works. For this project, we used Dear IMGUI with GLFW and Opengl for the graphical interface.

This project has two separate versions accessible via different branches : Raytracing version and Rasterization version (opengl).

### Raytracing version (on the “main” and “raycasting” branches)

This version was created to implement the software without a graphics API (only in C++) and with simple raytracing. Opengl and GLFW were used only for the interface.

It has the following features: 

#### Simple 3D scene
* Creation of a scene consisting of a mesh cube with the option to disable the grid
* Move the camera around the scene using the arrow keys (with speed options)
* View the scene's mesh tree and hide meshs
* Add or remove an mesh in the scene with a choice of ten basic meshs (cube, plane, triangle, cone, sphere, icosphere, cylinder, donut, monkey)
* Select a mesh in the scene to move, rotate, or resize it
* Save the scene in .obj format

#### Edit Mode
* Switch to “edit mode” after selecting a mesh to select or move the face or vertex of a mesh
* Several extrude options on a face of a mesh in “edit mode”

#### Render Mode
* Activate “render” mode to see the result of ray tracing on the scene with light.
* Add a skybox with the option to change it.
* Use Phong for light management on meshs with the ability to change the color, kd, ks, and ns of a mesh's material.
* Option to change the light intensity.
* Option to save a render in .ppm format.

### Rasterization version (on the “dev” and “opengl” branches)

The goal of this version was to port the raytracing version to opengl in order to run the software in real time, while adding more modern features. All the features of the raytracing version have been ported to opengl (simple 3D scene, Edit Mode, Render Mode). Here are the new features.

#### Simple 3D scene
* Camera movement is now in real time with options to change the zoom and rotation speed.
* The mesh tree now shows more options on the mesh as well as a sub-tree with the faces of the mesh.
* In addition to adding and deleting a mesh, you can now duplicate a mesh.
* Mesh selection is represented by an outline tracing the contours of the mesh faces.
* Mesh movement is in real time.

#### Render modes
* The “Raycast Render” button provides access to simple raycasting rendering (not real time).
* The “Normals” button switches to classic real-time rendering of 3D modeling software using the mesh normals.
* The “Outlines” button switches to real-time rendering using outlines only.
* The “Phong” button switches to real-time Phong rendering.
* The “BRDF” button switches to real-time PBR rendering with a Labertian diffuse BRDF and a Cook-Torrance GGX specular BRDF.
* The options for changing the materials of a mesh for Phong and PBR are in real time.

#### Bonus Features
* Option to simulate hair in a very simplified way using tessellation shader with the “fur only” button or the “fur” checkbox. Addition of customization options (fur length, fur size, and tessellation surface).
* Option to distort meshes using sinusoids with the “wave” button. It is possible to distort a mesh with sinusoids in all directions with any dependency, amplitude, and frequency (can be combined with hair simulation).

For this version of the software, we used modern rasterization light management techniques (PBR) as well as advanced OpenGL features (geometry shader, tessellation shader).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

The project is in CPP and the interface has been developped with IMGUI/OPGL/GLFW.
* [![Cpp][Cpp.cpp]][Cpp-url]
* [![OpenGL][OP.GL]][OPGL-url]
* [![ImGUI][IM.GUI]][IMGUI-url]
* [![GLFW][GL.FW]][GLFW-url]

## Getting Started

This project has been developped on Windows10 and Linux and should work on both. (Not tested on MacOs)

### Prerequisites

`Opengl 4.5` and `Glfw` are required to run this project

* Windows :
You can use `vcpkg` to install `glfw`, `glew` and `glm` on windows (it's very simple to use with Jet Brains Clion)

* Linux :
[https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project]

### Installation

1. Clone the repo
```sh
git clone https://github.com/Auxemite/Blender_eco
```

1.5. Change branch (if you want to test the rasterization version)
```sh
git switch dev
```

2. Build
```sh
cmake -B build .
cp imgui.ini build/imgui.ini
```

3. Run
``` sh
cd build
./blender_eco
```

## Important Info

For a more efficient way of using DEAR IMGUI, we have created a directory in src/frontend called "imgui". This is the code of DEAR IMGUI and is NOT our code. We use this library only for some help on the creation of the app.

<!-- AUTHORS -->
## Authors
Ernest Bardon
Kael Facon

<!-- MARKDOWN LINKS & IMAGES -->
<!--5586a6-->
[OP.GL]: https://img.shields.io/badge/opengl-FFFFFF?logo=opengl&style=for-the-badge
[OPGL-url]: https://opengl.org/

[Cpp.cpp]: https://img.shields.io/badge/c++-00599C?logo=c%2B%2B&style=for-the-badge
[Cpp-url]: https://www.cppreference.com/

[IM.GUI]: https://img.shields.io/badge/IMGUI-151617?logo=imgui&style=for-the-badge&logoColor=white
[IMGUI-url]: https://github.com/ocornut/imgui

[GL.FW]: https://img.shields.io/badge/GLFW-ff9a29?logo=glfw&style=for-the-badge
[GLFW-url]: https://glfw.org/
