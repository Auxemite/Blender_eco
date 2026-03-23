#pragma once

#include "inputs.hh"
#include "env.hh"
#include "app_utils.hh"

static const char* eye[] = {"<Ø>", "<O>"};

void TreeNode(Env& env);
void treeMesh(Env& env, int index);
void printObjInfo(Mesh* focus_mesh);