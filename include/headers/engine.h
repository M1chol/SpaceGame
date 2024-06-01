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

// Global engine variables
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Scene *mainScene;
extern std::vector<Scene *> sceneList;
extern double deltaTime;
extern double drawTime;
extern bool drawHitbox;
extern int nrOfScenes;

/*Initialize SDL wrapper "Engine" @return true if successfull*/
bool EngineInit();
// Close all Engine systems and SDL
void EngineClose();
// Updates arrays of active keys, it needs to be called each frame
void EngineUpdateKeyboard();
/* Used to cap frames per second of application @param targetFrames FPS limit */
void EngineCapFrames(int);
/* Check if key is beeing held @param key key to check @return true if key is beeing pressed */
bool isKeyDown(SDL_Scancode);
/* Check if key has been released @param key key to check @return true if key has been released */
bool isKeyReleased(SDL_Scancode);
/* Check if key just has been pressed @param key key to check @return true if key just has been pressed*/
bool isKeyPushed(SDL_Scancode);

#endif