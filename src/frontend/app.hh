#pragma once

#include "imgui/imgui.h"

class App {
    public:

    App();

    void Windows();
    void Inputs(const ImGuiIO& io, ImVec2 pos);
};