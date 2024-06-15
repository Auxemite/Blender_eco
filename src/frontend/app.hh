#pragma once

#include "env.hh"

class App {
    public:
    Env env;

    App();
    explicit App(const char* filename);

    void Windows();
    void MainOptions();
    void CameraOption();
    void Material();
    void SelectMesh(const ImGuiIO& io, ImVec2 pos);
    void TreeMesh(Mesh *mesh, int index);
    void TreeNode();
    void MeshOptions();
    void PrintObjInfo();
};