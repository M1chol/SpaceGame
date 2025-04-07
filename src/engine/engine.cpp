#include "engine.h"

// Global SDL variables and engine-related globals.
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 810;
std::vector<Scene *> sceneList;
double deltaTime = 0.0;
double drawTime = 0.0;
int nrOfScenes = 0;
int nrOfLayouts = 0;
iVect mousePos;
const Uint8 *currentKeyState = nullptr;
Uint8 previousKeyState[SDL_NUM_SCANCODES] = {0};
Uint32 mouseState = 0;
std::string fontVideo = "res/HomeVideo.ttf";
std::string fontVideoBold = "res/HomeVideoBold.ttf";
std::string fontSans = "res/OpenSans-Regular.ttf";
LuaManager *lua = nullptr;

// Setup flags
bool drawDebug = true;
bool waitToDebug = true;
bool showDebugNames = false;
bool gameLoop = true;

#pragma region Engine

gEngine::gEngine() : m_requestClose(false), timer(0) {}

gEngine::~gEngine()
{
    // Ideally, call close() first to ensure cleanup.
}

gEngine &gEngine::getInstance()
{
    static gEngine instance;
    return instance;
}

bool gEngine::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log(LOG_ERR) << "SDL could not initialize successfully: " << SDL_GetError()
                     << "\n";
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow)
    {
        log(LOG_ERR) << "Failed creating window: " << SDL_GetError() << "\n";
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer)
    {
        log(LOG_ERR) << "Failed to create renderer: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize SDL_Image
    int imageFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imageFlags) & imageFlags))
    {
        log(LOG_ERR) << "SDL_Image failed to initialize: " << IMG_GetError()
                     << "\n";
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        log(LOG_ERR) << "SDL_ttf failed to initialize: " << TTF_GetError() << "\n";
        return false;
    }

    // Create LuaManager instance
    lua = new LuaManager();
    if (!lua)
    {
        log(LOG_ERR) << "Lua failed to load\n";
        return false;
    }

    currentKeyState = SDL_GetKeyboardState(NULL);

    previousTime = SDL_GetTicks();
    currentTime = 0;

    return true;
}

void gEngine::update()
{
    currentTime = SDL_GetTicks();
    deltaTime = static_cast<double>(currentTime - previousTime) / 1000.0;

    // Update engine events (keyboard, mouse, etc.)
    updateEvents();

    // Update scenes.
    updateScenes();

    // Calculate draw time.
    drawTime = static_cast<double>(SDL_GetTicks() - currentTime) / 1000.0;

    previousTime = currentTime;
}

void gEngine::capFrames(int targetFrames)
{
    double targetFrameTime = 1.0 / (double)targetFrames;
    if (targetFrameTime > deltaTime)
    {
        SDL_Delay((Uint32)((targetFrameTime - deltaTime) * 1000));
        deltaTime = targetFrameTime;
    }
    if (deltaTime >= drawTime - drawTime * 0.05 && deltaTime < drawTime)
    {
        log(LOG_WARN) << "Game running at hardware limit!\n";
    }
    if (timer < 5.0)
    {
        timer += deltaTime;
    }
    else
    {
        log(LOG_INFO) << "Current frame rate " << (1.0 / deltaTime) << " fps\n";
        timer = 0.0;
    }
}

void gEngine::close()
{
    // Clean up scenes
    for (int i = nrOfScenes - 1; i >= 0; i--)
    {
        Scene *scene = sceneList[i];
        log(LOG_INFO) << "Destroying Scene " << scene->getName() << "... ("
                      << scene << ")\n";
        delete scene;
        sceneList[i] = nullptr;
        log(LOG_INFO) << "Scene Destroyed\n";
    }
    sceneList.clear();
    log(LOG_INFO) << "Quit successful, bye bye!\n";

    // Quit SDL and related libraries
    SDL_Quit();
}

void gEngine::requestClose()
{
    log(LOG_INFO) << "Quit requested...\n";
    gameLoop = false;
    m_requestClose = true;
}

void gEngine::updateEvents()
{
    SDL_PumpEvents();
    mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
    memcpy(previousKeyState, currentKeyState, SDL_NUM_SCANCODES);
}

bool gEngine::isKeyDown(SDL_Scancode key)
{
    return currentKeyState[key];
}

bool gEngine::isKeyDown(MouseButton button)
{
    return mouseState & SDL_BUTTON(button);
}

bool gEngine::isKeyReleased(SDL_Scancode key)
{
    return previousKeyState[key] && !currentKeyState[key];
}

bool gEngine::isKeyPushed(SDL_Scancode key)
{
    return currentKeyState[key] && !previousKeyState[key];
}

Scene *gEngine::addScene(const std::string &name)
{
    Scene *newScene = new Scene();
    newScene->setName(name);
    // Assume that scenes are unsorted upon adding a new scene.
    sceneList.push_back(newScene);
    nrOfScenes++;
    return newScene;
}

int gEngine::getSceneID(Scene *scene)
{
    for (int i = 0; i < nrOfScenes; i++)
    {
        if (scene == sceneList[i])
        {
            return i;
        }
    }
    return -1;
}

void gEngine::updateScenes()
{
    if (nrOfScenes < 1)
    {
        return;
    }
    if (nrOfScenes == 1)
    {
        sceneList[0]->Update();
        return;
    }

    // Sort scenes based on draw priority if necessary.
    if (nrOfScenes > (int)sceneList.size())
    {
        std::sort(sceneList.begin(), sceneList.end(), [](Scene *a, Scene *b)
                  { return a->getDrawPriority() < b->getDrawPriority(); });
    }

    sceneList[0]->Update(true);
    for (int i = 1; i < nrOfScenes - 1; i++)
    {
        sceneList[i]->Update(true, true);
    }
    sceneList[nrOfScenes - 1]->Update(false, true);
}

Scene *gEngine::getSceneByName(const std::string &name)
{
    for (auto &scene : sceneList)
    {
        if (scene->getName() == name)
        {
            return scene;
        }
    }
    log(LOG_WARN) << "Scene " << name << " not found\n";
    return nullptr;
}

#pragma endregion

#pragma region LuaManager

LuaManager::LuaManager()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    log(LOG_INFO) << "Lua loaded successfully\n";
}

LuaManager::~LuaManager()
{
    if (L)
    {
        lua_close(L);
        log(LOG_INFO) << "Lua closed\n";
    }
}

bool LuaManager::run(const char *filename)
{
    int status = luaL_dofile(L, filename);
    if (status != LUA_OK)
    {
        log(LOG_WARN) << lua_tostring(L, -1) << "\n";
        return false;
    }
    return true;
}
