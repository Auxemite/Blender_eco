#pragma once

#include "env.hh"

class App {
    public:
    Env env;

    App();
    explicit App(const char* filename);

    void Windows();
    void TreeMesh(Mesh *mesh);
    void TreeNode();
    void MeshOptions();
    void PrintObjInfo();
};