#include <engine.h>

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

// Setup flags
bool drawDebug = true;
bool waitToDebug = true;
bool showDebugNames = false;
bool gameLoop = true;
bool useSafeLuaCall = true;

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
    log(LOG_INFO) << "SDL init complete\n";

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
    log(LOG_INFO) << "Engine init complete\n";
    return true;
}

void gEngine::updateEngine()
{
    if (isKeyPushed(SDL_SCANCODE_F5))
    {
        lua->reload();
    }

    // Set previous state for keyboard
    memcpy(previousKeyState, currentKeyState, SDL_NUM_SCANCODES);
}

void gEngine::update()
{
    currentTime = SDL_GetTicks();
    deltaTime = static_cast<double>(currentTime - previousTime) / 1000.0;

    // Update engine events (keyboard, mouse, etc.)
    updateEvents();

    // Update scenes.
    updateScenes();

    updateEngine();

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
        log(LOG_INFO) << "Scene Destroyed (" << sceneList[i]->getName() << ")\n";
        delete sceneList[i];
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
}

bool gEngine::isKeyDown(SDL_Scancode key)
{
    return currentKeyState[key];
}

bool gEngine::isKeyDown(MouseButton button)
{
    return mouseState & SDL_BUTTON(button);
}
// FIXME: key released not working
bool gEngine::isKeyReleased(SDL_Scancode key)
{
    return previousKeyState[key] && !currentKeyState[key];
}

// FIXME:  key pushed not working
bool gEngine::isKeyPushed(SDL_Scancode key)
{
    return currentKeyState[key] && !previousKeyState[key];
}

Scene *gEngine::addScene(const std::string &name)
{
    Scene *newScene = new Scene();
    log(LOG_INFO) << "Added Scene " << newScene << "\n";
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

LuaManager::LuaManager() : Lstate(sol::default_at_panic)
{
    reload();
    defineLuaObjects();
    log(LOG_INFO) << "Lua init complete\n";
}

LuaManager::~LuaManager()
{
    log(LOG_INFO) << "Lua closed\n";
}

void LuaManager::reload()
{
    log(LOG_INFO) << "LUA reload initiated\n";
    // Create new state
    Lstate = sol::state();

    // Link existing scenes
    for (auto &scene : sceneList)
    {
        Lstate[scene->getName()] = scene;
    }

    // TODO: Not sure about those libraries
    Lstate.open_libraries(sol::lib::base, sol::lib::io);
    defineLuaObjects();

    for (auto &path : scriptList)
    {
        sol::load_result chunk = Lstate.load_file(path);
        if (!chunk.valid())
        {
            sol::error err = chunk;
            log(LOG_ERR) << "LUA error loading " << err.what() << "\n";
            continue;
        }
        sol::protected_function_result result = chunk();
        if (!result.valid())
        {
            sol::error err = result;
            log(LOG_ERR) << "LUA error running " << err.what() << "\n";
        }
    }
}

bool LuaManager::run(const char *filename)
{
    scriptList.push_back(filename);
    reload();
}

void LuaManager::defineLuaObjects()
{
    Lstate.new_usertype<Vect>("Vect",
                              // Constructors using sol::constructors.
                              sol::constructors<Vect(), Vect(double, double)>(), "x", &Vect::x, "y", &Vect::y,
                              // Bind operator+ via a lambda (note: a and b are now const)
                              sol::meta_function::addition, [](const Vect &a, const Vect &b)
                              { return a + b; },
                              // Similarly for subtraction:
                              sol::meta_function::subtraction, [](const Vect &a, const Vect &b)
                              { return a - b; },
                              // Multiplication (by a scalar)
                              sol::meta_function::multiplication, [](const Vect &a, double s)
                              { return a * s; }, "magnitude", &Vect::magnitude, "normalized", &Vect::normalized, "rotate", &Vect::rotate, "toIVect", &Vect::toIVect);

    // Expose iVect
    Lstate.new_usertype<iVect>("iVect", sol::constructors<iVect(), iVect(int, int)>(), "x", &iVect::x, "y", &iVect::y, sol::meta_function::addition, [](const iVect &a, const iVect &b)
                               { return a + b; }, sol::meta_function::multiplication, [](const iVect &a, int s)
                               { return a * s; });

    // Expose Component (assuming abstract usage is OK)
    Lstate.new_usertype<Component>("Component",
                                   "getParent", &Component::getParent,
                                   "getName", &Component::getName);

    // Expose Object (only exposing selected functions)
    Lstate.new_usertype<Object>("Object",
                                sol::constructors<Object(Scene *), Object(Object *)>(),
                                "destroy", &Object::destroy,
                                "addTag", &Object::addTag,
                                "removeComponent", &Object::removeComponent,
                                "getChildByName", &Object::getChildByName,
                                "setName", &Object::setName,
                                "getName", &Object::getName,
                                "move", &Object::simpleMove,
                                "rotate", &Object::rotate,
                                "getRotation", &Object::getRotation,
                                "getPos", &Object::getPos);

    // Expose Scene (be sure that SDL_Renderer is complete)
    Lstate.new_usertype<Scene>("Scene",
                               sol::constructors<Scene(std::string)>(),
                               "setName", &Scene::setName,
                               "getName", &Scene::getName,
                               "getObjectByName", &Scene::getObjectByName,
                               "getObjectsByTag", &Scene::getObjectsByTag,
                               "getNrOfObjects", &Scene::getNrOfObjects,
                               "setDrawPriority", &Scene::setDrawPriority,
                               "getDrawPriority", &Scene::getDrawPriority);

    Lstate.new_usertype<SpriteComponent>("SpriteComponent",
                                         sol::constructors<SpriteComponent(std::string, Object *)>(),
                                         "load", &SpriteComponent::load,
                                         "render", sol::overload(static_cast<bool (SpriteComponent::*)(int, iVect, float)>(&SpriteComponent::render), static_cast<bool (SpriteComponent::*)(int, float)>(&SpriteComponent::render), static_cast<bool (SpriteComponent::*)()>(&SpriteComponent::render)),
                                         "whenLinked", &SpriteComponent::whenLinked,
                                         "getDim", &SpriteComponent::getDim,
                                         "setScale", &SpriteComponent::setScale,
                                         "setSheetIndex", &SpriteComponent::setSheetIndex,
                                         "setOffset", &SpriteComponent::setOffset,
                                         "setCentered", &SpriteComponent::setCentered,
                                         "rotate", &SpriteComponent::rotate,
                                         "getCenter", &SpriteComponent::getCenter);

    // Expose RigidBodyComponent
    Lstate.new_usertype<RigidBodyComponent>("RigidBodyComponent",
                                            sol::constructors<RigidBodyComponent(double, Object *)>(),
                                            "update", &RigidBodyComponent::update,
                                            "render", &RigidBodyComponent::render,
                                            "whenLinked", &RigidBodyComponent::whenLinked,
                                            "applyForce", &RigidBodyComponent::applyForce,
                                            "setMass", &RigidBodyComponent::setMass,
                                            "setEnergyLoss", &RigidBodyComponent::setEnergyLoss,
                                            "setCollision", &RigidBodyComponent::setCollision,
                                            "getHitBox", sol::overload(static_cast<SDL_Rect *(RigidBodyComponent::*)()>(&RigidBodyComponent::getHitBox), static_cast<iVect (RigidBodyComponent::*)(int)>(&RigidBodyComponent::getHitBox)),
                                            "isColliding", sol::overload(static_cast<RigidBodyComponent *(RigidBodyComponent::*)(RigidBodyComponent *)>(&RigidBodyComponent::isColliding), static_cast<RigidBodyComponent *(RigidBodyComponent::*)(TAG)>(&RigidBodyComponent::isColliding)),
                                            "solveCollision", &RigidBodyComponent::solveCollision,
                                            // Expose hasCollision as a read-only property:
                                            "hasCollision", sol::property([](RigidBodyComponent &self)
                                                                          { return self.hasCollision; }));

    // Expose TextComponent (derived from SpriteComponent)
    Lstate.new_usertype<TextComponent>("TextComponent",
                                       sol::constructors<TextComponent(std::string, iVect, std::string, int, Object *)>(),
                                       "setColor", &TextComponent::setColor,
                                       "setScale", &TextComponent::setScale, // Note: Shadows SpriteComponent setScale
                                       "setSize", &TextComponent::setSize,
                                       "setFont", &TextComponent::setFont);

    Lstate.new_usertype<uiSphere>("uiSphere",
                                  sol::constructors<uiSphere(Object *, float, float)>(),
                                  "setColor", &uiSphere::setColorSimple);

    Lstate.new_usertype<uiRoundedRect>("uiRoundedRect", sol::constructors<uiRoundedRect(Object *, int, float, float, float)>(),
                                       "setColor", &uiRoundedRect::setColorSimple

    );
}