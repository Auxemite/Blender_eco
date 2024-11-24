#pragma once

#include "env.hh"
#include "inputs.hh"

static const char* mesh_names[] = { "Cube", "Plane", "Triangle", "Cone", "Sphere", "Icosphere", "Cylinder", "Donut", "Monkey"};
static const char* background_names[] = { "Sky", "Sunset", "Forest", "Synthwave", "Space", "Mars", "Earth"};

void saveFile(Mesh *focus_mesh);
void raycastRender(Env& env);
void colorSetup(ImVec4& color);
void addMesh(Env& env);