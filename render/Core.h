#ifndef CORE_H
#define CORE_H

#include "Kernel.h"
#include "Packet.h"
#include "BasicShape.h"
#include <vector>
#include "Scene.h"

Scene* scene;
Scene* sceneAux;

void init(std::vector<BasicShape*> obj);

void sendPackage(Package* packet);

#endif