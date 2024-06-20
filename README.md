AUTHORS
Ernest Bardon
Kael Facon

## Set up and launch

Opengl 3 and Glfw are required to run this project

You can use vcpkg to install glfw on windows (it's very simple to use with Jet Brains Clion

If everything is ok, run :
cmake -B build .

Then go into the new created directory "build" and run the executable file to start the app

## Important Info

For a more efficient way of using DEAR IMGUI, we have created a directory in src/frontend called "imgui". This is the code of DEAR IMGUI and is NOT our code. We use this library only for some help on the creation of the app. We also used some parts of tutorial window given by the library to help us create the src/frontend/app.cpp and the main.cpp
