#pragma once

#include "guiUtils.hh"
#include "backend/scene.hh"
#include "graphics/grid.hh"

namespace Gui {

    static std::vector<std::string> mesh_names = {};
    void mainGui(Scene *scene, VisualGrid& grid);
    void editMode(Scene *scene, VisualGrid& grid);
    void meshTreeNode(Scene *scene);
    void addMesh(Scene *scene);
    void deleteMesh(Scene *scene);
    void duplicateMesh(Scene *scene);
    void materialsAndTextures(Scene *scene);
    void lights(Scene *scene);
}