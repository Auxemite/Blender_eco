AUTHORS
Ernest Bardon
Kael Facon
Thomas Xu

## Presentation
This is a project made by 4th year students of Epita. The goal is to create an opensource software for 3D modelisation. 

## Set up and launch

Opengl 3, Glfw, Glew and Glm are required to run this project

You can use vcpkg to install glfw glew and glm on windows (it's very simple to use with Jet Brains Clion)

If everything is ok, run :
cmake -B build .

Then go into the new created directory "build" and run the executable file to start the app

## Important Info

For a more efficient way of using DEAR IMGUI, we have created a directory in src/frontend called "imgui". This is the code of DEAR IMGUI and is NOT our code. We use this library only for some help on the creation of the app.
