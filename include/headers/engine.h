#ifndef Engine
#define Engine
#pragma once

#include <string>
#include <vector>
#include "logger.h"
#include <SDL.h>
#include <SDL_image.h>
#include "gPrimitives.h"
#include "gComponents.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Scene *mainScene;
extern std::vector<Scene *> sceneList;

bool EngineInit();
void EngineClose();

#endif