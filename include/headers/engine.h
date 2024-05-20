#ifndef Engine
#define Engine
#pragma once

#include <string.h>
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
extern double deltaTime;

bool EngineInit();
void EngineClose();
void EngineUpdateKeyboard();
void EngineCapFrames(int frames);
/*
Checks status of key and returns true if it is beeing pressed
@param key Key to check
@return True if key is beeing pressed
*/
bool isKeyDown(SDL_Scancode key);


bool isKeyReleased(SDL_Scancode);

bool isKeyPushed(SDL_Scancode);

#endif