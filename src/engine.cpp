#include "engine.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 810;
std::vector<Scene *> sceneList;
double deltaTime;
double drawTime;
int nrOfScenes = 0;
int nrOfLayouts = 0;
iVect mousePos;
const Uint8 *currentKeyState;
Uint8 previousKeyState[SDL_NUM_SCANCODES];
Uint32 mouseState;
std::string fontVideo = "res/HomeVideo.ttf";
std::string fontVideoBold = "res/HomeVideoBold.ttf";
std::string fontSans = "res/OpenSans-Regular.ttf";

// SETUPS
bool drawDebug = true;
bool waitToDebug = true;
bool showDebugNames = false;
bool gameLoop = true;
bool EngineInit()
{

    // Try Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log(LOG_ERR) << "SDL could not initialize successfuly " << SDL_GetError() << "\n";
        return false;
    }
    // Try to create window
    gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        log(LOG_ERR) << "Failed creating window " << SDL_GetError() << "\n";
        return false;
    }
    // Try to create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        log(LOG_ERR) << "Failed to create renderer " << SDL_GetError() << "\n";
        return false;
    }

    // Try to init SDL_Image
    int imageFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imageFlags) & imageFlags))
    {
        log(LOG_ERR) << "SDL_Image failed to initialize " << IMG_GetError() << "\n";
        return false;
    }
    // Try to init SDL_ttf
    if (TTF_Init() == -1)
    {
        log(LOG_ERR) << "SDL_ttf failed to initialize" << TTF_GetError() << "\n";
        return false;
    }

    currentKeyState = SDL_GetKeyboardState(NULL);

    return true;
}

void EngineCapFrames(int targetFrames)
{

    double targetFrameTime = 1 / (double)targetFrames;
    if (targetFrameTime > deltaTime)
    {
        SDL_Delay((Uint32)((targetFrameTime - deltaTime) * 1000));
        deltaTime = targetFrameTime;
    }
    if (drawTime - drawTime * 0.05 <= deltaTime < drawTime)
    {
        log(LOG_WARN) << "Game running at hardware limit!\n";
    }
}

void EngineClose()
{

    for (int i = nrOfScenes - 1; i >= 0; i--)
    {
        Scene *scene = sceneList[i];
        log(LOG_INFO) << "Destroying Scene " << scene->getName() << "... (" << scene << ")\n";
        delete scene;
        scene = nullptr;
        log(LOG_INFO) << "Scene Destroyed\n";
    }
    sceneList.clear();
    log(LOG_INFO) << "Quit successfull, bye bye!\n";
    SDL_Quit();
}
void requestEngineClose()
{
    gameLoop = false;
}

int LayoutGetID()
{
    return nrOfLayouts++;
}

#pragma region KEYBOARD

void EngineUpdateEvents()
{
    SDL_PumpEvents();
    mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
    memcpy(previousKeyState, currentKeyState, SDL_NUM_SCANCODES);
}

bool isKeyDown(SDL_Scancode key)
{
    return currentKeyState[key];
}

bool isKeyDown(MouseButton button)
{
    return mouseState & SDL_BUTTON(button);
}

bool isKeyReleased(SDL_Scancode key)
{
    return previousKeyState[key] && !currentKeyState[key];
}

// TODO: check if it even works
bool isKeyPushed(SDL_Scancode key)
{
    return currentKeyState[key] && !previousKeyState[key];
}
#pragma endregion

#pragma region SCENE SETUP

// Scene *addScene(std::string name)
// {
//     Scene *newScene = new Scene();
//     newScene->setName(name);
//     scenesSorted = false;
//     sceneList.push_back(newScene);
//     nrOfScenes++;
//     return newScene;
// }

int getSceneID(Scene *scene)
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

Scene *getSceneByName(std::string name)
{
    for (auto &scene : sceneList)
        if (scene->getName() == name)
            return scene;
    log(LOG_WARN) << "Scene " << name << " not found\n";
    return nullptr;
}

void EngineUpdateScenes()
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

    if (nrOfScenes > sceneList.size())
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

#pragma endregion

void centerRect(SDL_Rect *box)
{
    box->x = box->w / 2;
    box->y = box->h / 2;
}