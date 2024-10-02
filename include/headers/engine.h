#ifndef Engine
#define Engine
#pragma once

enum TAG
{
    TAG_PLAYER,
    TAG_ENEMY
};
#define LOG_INFO "\x1B[34m[ INF ]\033[0m"
#define LOG_ERR "\x1B[31m[ ERR ]\033[0m"
#define LOG_WARN "\x1B[33m[ WRN ]\033[0m"

// Core files
#include <iostream>

#include <algorithm>
#include <cstring>
#include <sstream>
#include <chrono>
#include <memory>
#include <math.h>
#include <ctime>
#include <iomanip>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <logger.h>
#define log(name) Logger(name)

#include <gPrimitives.h>
#include <gComponents.h>
#include <gLayouts.h>

// Prefab files
#include <genericBullet.h>

// Global engine variables
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Scene *mainScene;
extern std::vector<Scene *> sceneList;
extern double deltaTime;
extern double drawTime;
extern bool drawHitbox;
extern bool waitToDebug;
extern bool showDebugNames;
extern int nrOfScenes;
extern std::string globalFont;

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