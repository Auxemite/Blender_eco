#pragma once

#include "env.hh"
#include "inputs.hh"
#include <filesystem>

static std::vector<std::string> mesh_names = {};
static std::vector<std::string> background_names = {};

void saveFile(Mesh *focus_mesh);
void raycastRender(Env& env);
void colorSetup(ImVec4& color);
void addMesh(Env& env);