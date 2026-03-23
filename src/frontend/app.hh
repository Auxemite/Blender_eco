#pragma once

#include "app_utils.hh"
#include "ui_tree.hh"
#include "mesh_action.hh"

class App {
    public:
    Env env;

    App();

    void Windows();
    void MainOptions();
    void SpecialOptions();
    void ShadersOptions();
    void Material();
    void MeshOptions();
};