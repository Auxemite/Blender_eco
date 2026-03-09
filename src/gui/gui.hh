#pragma once

#include "guiUtils.hh"
#include "backend/scene.hh"

namespace Gui {
    static std::vector<std::string> mesh_names = {};
    void mainGui(Scene *scene);
    void meshTreeNode(Scene *scene);
    void addMesh(Scene *scene);
    void deleteMesh(Scene *scene);
    void duplicateMesh(Scene *scene);
}