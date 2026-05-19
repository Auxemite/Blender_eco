#pragma once

#include "guiUtils.hh"
#include "scene/scene.hh"
#include "graphics/grid.hh"
#include "editmode/editmodeScene.hh"

namespace Gui {

    static std::vector<std::string> mesh_names = {};
    void mainGui(Scene& scene, EditMode::EditModeScene& editModeScene, VisualGrid& grid);
    void meshTreeNode(Scene& scene);
    void addMesh(Scene& scene);
    void materials(Scene& scene);
    void textures(Scene& scene);
    void lights(Scene& scene);
}