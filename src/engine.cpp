#include "engine.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
std::vector<Scene *> sceneList;
Scene *mainScene = nullptr;
double deltaTime;
double drawTime;

Uint8 currentKeyState[SDL_NUM_SCANCODES];
Uint8 previousKeyState[SDL_NUM_SCANCODES];

bool EngineInit()
{
    LOG_INIT_CERR();

    // Try Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log(LOG_ERROR) << "SDL could not initialize successfuly " << SDL_GetError() << "\n";
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
    mainScene = new Scene(gRenderer);
    sceneList.push_back(mainScene);
    mainScene->setName("MAIN");
    log(LOG_INFO) << "Initilization succesfull created main Scene\n";
    return true;
}

void EngineUpdateKeyboard()
{
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    memcpy(previousKeyState, currentKeyState, SDL_NUM_SCANCODES);
    memcpy(currentKeyState, state, SDL_NUM_SCANCODES);
}

void EngineCapFrames(int targetFrames)
{
    static LOG_INIT_CERR();
    double targetFrameTime = 1 / (double)targetFrames;
    if (targetFrameTime > deltaTime)
    {
        SDL_Delay((Uint32)((targetFrameTime - deltaTime) * 1000));
        deltaTime = targetFrameTime;
    }
    if (drawTime-drawTime*0.05 <= deltaTime < drawTime)
    {
        log(LOG_WARN) << "Game running at hardware limit!\n";
    }
}

bool isKeyDown(SDL_Scancode key)
{
    return currentKeyState[key];
}

bool isKeyReleased(SDL_Scancode key)
{
    return previousKeyState[key] && !currentKeyState[key];
}
bool isKeyPushed(SDL_Scancode key)
{
    return currentKeyState[key] && !previousKeyState[key];
}

void EngineClose()
{
    LOG_INIT_CERR();
    for (auto &scene : sceneList)
    {
        scene->destroy();
    }
    SDL_Quit();
    log(LOG_INFO) << "Quit successfull, bye bye!\n";
}