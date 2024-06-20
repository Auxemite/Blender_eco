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
    void Inputs(const ImGuiIO& io, ImVec2 pos);
    void Add_Mesh();
    void TreeMesh(Mesh *mesh, int index);
    void TreeNode();
    void MeshOptions();
    void PrintObjInfo() const;
};