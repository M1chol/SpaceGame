#ifndef Engine
#define Engine
#pragma once

enum TAG
{
    TAG_PLAYER,
    TAG_ENEMY
};

enum ComponentType
{
    COMP_TYPE_SPRITE,
    COMP_TYPE_RIGIDBODY,
    COMP_TYPE_SPAWNER,
    COMP_TYPE_TEXT,
    COMP_TYPE_LAYOUT
};
enum SpawnerBulletType
{
    SPAWNER_BULLET_GENERIC
};

enum LoadFlag
{
    OBJECT,
    GRID
};

// Core libraries
#include <iostream>

#include <algorithm>
#include <cstring>
#include <sstream>
#include <fstream>
#include <chrono>
#include <memory>
#include <math.h>
#include <ctime>
#include <iomanip>
#include <vector>

// Logger utility
#include <logger.h>
#define log(name) Logger(name)
#define LOG_INFO "\x1B[34m[ INF ]\033[0m"
#define LOG_ERR "\x1B[31m[ ERR ]\033[0m"
#define LOG_WARN "\x1B[33m[ WRN ]\033[0m"
#define LOG_OK "\x1B[32m[ SUC ]\033[0m"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <gPrimitives.h>
#include <gComponents.h>
#include <gLayouts.h>

// Prefab files
#include <genericBullet.h>

// Global engine variables
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern std::vector<Scene *> sceneList;
extern double deltaTime;
extern double drawTime;
extern bool drawDebug;
extern bool waitToDebug;
extern bool showDebugNames;
extern int nrOfScenes;
extern std::string globalFont;
extern int nrOfLayouts;

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
/* Add scene to sceneList */
Scene *addScene(std::string name);
/* Returns the id of Scene*/
int getSceneID(Scene *scene);

int LayoutGetID();

bool saveBin(Object *obj, std::string filename);
Object *loadBin(std::string filename, Scene *scene);

void centerRect(SDL_Rect *box);

#endif