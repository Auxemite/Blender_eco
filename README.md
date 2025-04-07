## Set up and launch

Opengl 3 and Glfw are required to run this project.

You can use vcpkg to install glfw on windows (it's very simple to use with Jet Brains Clion).

If everything is ok, run :
cmake -B build .

Then go into the new created directory "build" and run the executable file to start the app.

## Information

You don't have to use the main.cpp which is the main for the use of the IMGUI library. You can start coding in the src/frontend/app.cpp.

For some examples you can look at the Demo Window which is simply the code written in src/frontend/imgui/imgui_demo.cpp.
