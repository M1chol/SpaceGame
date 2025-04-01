#ifndef Engine
#define Engine
#pragma once

enum TAG
{
    TAG_PLAYER,
    TAG_ENEMY
};

enum MouseButton
{
    MOUSE_NONE,
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT
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
#include <functional>

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

#include <lua.hpp>

#include <gPrimitives.h>
#include <gComponents.h>
#include <gLayouts.h>
#include <gUi.h>

// Prefab files
#include <genericBullet.h>

// Global engine variables
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern std::vector<Scene *> sceneList;
extern SDL_Renderer *gRenderer;
extern double deltaTime;
extern double drawTime;
extern bool drawDebug;
extern bool waitToDebug;
extern bool showDebugNames;
extern bool gameLoop;
extern int nrOfScenes;
extern int nrOfLayouts;
// Current position of a mouse
extern iVect mousePos;

extern std::string fontSans;
extern std::string fontVideo;
extern std::string fontVideoBold;

class LuaManager
{
public:
    ~LuaManager();
    LuaManager();
    bool run(const char *filename);

private:
    lua_State *L;
};

extern LuaManager *lua;

// TODO: renake the system to use singleton class

/*Initialize SDL wrapper "Engine" @return true if successfull*/
bool EngineInit();
// Close all Engine systems and SDL
void EngineClose();
// Politely request engine to close
void requestEngineClose();
// Updates arrays of active keys, it needs to be called each frame
void EngineUpdateEvents();
/* Used to cap frames per second of application @param targetFrames FPS limit */
void EngineCapFrames(int);
/* Check if key is beeing held @param key key to check @return true if key is beeing pressed */
bool isKeyDown(SDL_Scancode);
/* Check if key is beeing held @param key key to check @return true if key is beeing pressed */
bool isKeyDown(MouseButton);
/* Check if key has been released @param key key to check @return true if key has been released */
bool isKeyReleased(SDL_Scancode);
/* Check if key just has been pressed @param key key to check @return true if key just has been pressed*/
bool isKeyPushed(SDL_Scancode);
/* Add scene to sceneList */
Scene *addScene(std::string name);
/* Returns the id of Scene*/
int getSceneID(Scene *scene);
// Draw all scenes based on drawPriority property
void EngineUpdateScenes();

Scene *getSceneByName(std::string name);

int LayoutGetID();

bool saveBin(Object *obj, std::string filename);
Object *loadBin(std::string filename, Scene *scene);

void centerRect(SDL_Rect *box);

#endif