#pragma once

#include "../backend/render/render.hh"
#include "env.hh"

class App {
    public:
    Env env;

    App();
    explicit App(const char* filename);

    void Windows();
};