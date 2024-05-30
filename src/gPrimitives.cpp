#include "engine.h"

#pragma region Vect math implementation

Vect Vect::operator*(double scalar)
{
    return Vect{x * scalar, y * scalar};
}
Vect Vect::operator+(const Vect &other)
{
    return Vect{x + other.x, y + other.y};
}
Vect &Vect::operator+=(const Vect &other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vect &Vect::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
double Vect::magnitude()
{
    return std::sqrt(x * x + y * y);
}
Vect Vect::normalized()
{
    double mag = magnitude();
    if (mag == 0)
    {
        return {0.0, 0.0};
    }
    else
    {
        return {x / mag, y / mag};
    }
}

#pragma endregion

#pragma region gObject definitions

Object::Object(Scene *scene)
{
    // Set default position
    linkedScene = scene;
    linkedScene->addObject(this);
    pos = {0, 0};
    name = "unnamed";
}
Object::~Object()
{
    destroy();
}
void Object::destroy()
{
    LOG_INIT_CERR();
    for (auto &component : componentList)
    {
        component->destroy();
    }
    componentList.clear();
    componentList.shrink_to_fit();
    if (!linkedScene->removeObject(this))
    {
        log(LOG_WARN) << "Could not remove " << name << " Object from linked scene\n";
    }
    linkedScene = nullptr;
}

Component *Object::getComponent(int componentId)
{
    LOG_INIT_CERR();
    if (componentId + 1 > componentList.size())
    {
        log(LOG_WARN) << "Trying to access not existant component in " << name << " Object\n";
        return nullptr;
    }
    return componentList[componentId];
}

void Object::addComponent(Component *comp)
{
    componentList.push_back(comp);
    comp->setParent(this);
    comp->whenLinked();
}
void Object::setScene(Scene *parentScene)
{
    linkedScene = parentScene;
}
Scene *Object::getScene()
{
    LOG_INIT_CERR();
    if (linkedScene != nullptr)
    {
        return linkedScene;
    }
    else
    {
        log(LOG_WARN) << "Trying to access linked scene that does not exist (" << name << ")\n";
        return nullptr;
    }
}
void Object::setName(std::string newName)
{
    name = newName;
}
std::string Object::getName()
{
    return name;
}

bool Object::removeComponent(Component *comp)
{
    auto el = std::find(componentList.begin(), componentList.end(), comp);
    if (el != componentList.end())
    {
        componentList.erase(el);
        return true;
    }
    return false;
}

void Object::render()
{
    for (auto &component : componentList)
    {
        component->render();
    }
}

void Object::update()
{
    for (auto &component : componentList)
    {
        component->update();
    }
}

#pragma endregion

#pragma region Component definitions

void Component::setParent(Object *new_parent)
{
    parent = new_parent;
}

bool Component::render() { return true; };
bool Component::update() { return true; };

Component::~Component()
{
    destroy();
}

void Component::whenLinked() {};
void Component::destroy() {};

#pragma endregion

#pragma region Scene definitions

Scene::Scene(SDL_Renderer *newRenderer)
{
    sceneList.push_back(this);
    nrOfActiveObjects = 0;
    name = "unnamed";
    sceneRenderer = newRenderer;
}
Scene::~Scene()
{
    destroy();
}
void Scene::destroy()
{
    LOG_INIT_CERR();
    for (auto &object : objectList)
    {
        object->destroy();
        log(LOG_INFO) << "Destroyed object " << object->getName() << " (" << object << ")\n";
    }
}
void Scene::setName(std::string newName)
{
    name = newName;
}
std::string Scene::getName()
{
    return name;
}

bool Scene::addObject(Object *obj)
{
    objectList.push_back(obj);
    // obj->setScene(this);
    nrOfActiveObjects++;
}

int Scene::Update()
{
    SDL_RenderClear(sceneRenderer);
    int temp = 0;
    for (auto &obj : objectList)
    {
        if (obj->isActive)
        {
            // TODO: calling the same 2 for loops. Will compilator fix that?
            temp++;
            obj->render();
            obj->update();
        }
    }
    SDL_RenderPresent(sceneRenderer);
    return temp;
}
SDL_Renderer *Scene::getRenderer()
{
    return sceneRenderer;
}
bool Scene::removeObject(Object *obj)
{
    auto el = std::find(objectList.begin(), objectList.end(), obj);
    if (el != objectList.end())
    {
        objectList.erase(el);
        return true;
    }
    return false;
}

#pragma endregion
