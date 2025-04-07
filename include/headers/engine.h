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

// m1 engine stuff
#include <gPrimitives.h>
#include <gComponents.h>
#include <gUi.h>

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

class gEngine
{
public:
    // Get the singleton instance.
    static gEngine &getInstance();

    // Initialize SDL wrapper and any other engine systems.
    bool init();

    // Close all engine systems and SDL.
    void close();

    void update();

    // Politely request the engine to close.
    void requestClose();

    // Update arrays of active keys
    void updateEvents();

    // Cap the frames per second of the application.
    // @param targetFrames: FPS limit.
    void capFrames(int targetFrames);

    // Input functions.
    // Check if key is being held.
    bool isKeyDown(SDL_Scancode key);
    // Check if mouse button is being pressed.
    bool isKeyDown(MouseButton button);
    // Check if key has been released.
    bool isKeyReleased(SDL_Scancode key);
    // Check if key has just been pressed.
    bool isKeyPushed(SDL_Scancode key);

    // Scene management.
    // Add a scene to the scene list.
    Scene *addScene(const std::string &name);
    // Get the ID of a scene.
    int getSceneID(Scene *scene);
    // Update all scenes based on their drawPriority property.
    void updateScenes();
    // Get a scene by its name.
    Scene *getSceneByName(const std::string &name);

    bool isRunning() const { return !m_requestClose; }

private:
    // Private constructor and destructor for singleton.
    gEngine();
    ~gEngine();

    // Internal flag to indicate a close request.
    bool m_requestClose;
    float timer;
    uint32_t previousTime;
    uint32_t currentTime;
};
#endif