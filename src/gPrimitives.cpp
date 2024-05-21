#include "engine.h"

// iVect toiVect(Vect vect){
//     return {(int)(vect.x + 0.5), (int)(vect.y+0.5)};
// }

#pragma region gObject definitions

Object::Object()
{
    // Set default position
    pos = {0, 0};
    linkedScene = nullptr;
}
Object::~Object()
{
    destroy();
}
void Object::destroy()
{
    for (auto &component : componentList)
    {
        delete component;
    }
    componentList.clear();
    linkedScene = nullptr;
}

Component *Object::getComponent(int componentId)
{
    LOG_INIT_CERR();
    if (componentId + 1 > componentList.size())
    {
        log(LOG_WARN) << "Trying to access not existant component\n";
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
void Object::removeComponent(Component *comp)
{
    auto el = std::find(componentList.begin(), componentList.end(), comp);
    if (el != componentList.end())
    {
        componentList.erase(el);
    }
}

void Object::render()
{
    int count = 0;
    for (auto &component : componentList)
    {
        component->render();
    }
}

void Object::update() {}

#pragma endregion

#pragma region Component definitions

void Component::setParent(Object *new_parent)
{
    parent = new_parent;
}

bool Component::render()
{
    return 0;
}

void Component::destroy() {};

Component::~Component()
{
    destroy();
}

void Component::whenLinked() {};

#pragma endregion

#pragma region Scene definitions

Scene::Scene(SDL_Renderer *newRenderer)
{
    sceneList.push_back(this);
    nrOfActiveObjects = 0;
    name = "Scene";
    sceneRenderer = newRenderer;
}
Scene::~Scene()
{
    destroy();
}
void Scene::destroy()
{
    for (auto &object : objectList)
    {
        delete object;
    }
}
void Scene::setName(std::string newName)
{
    name = newName;
}
bool Scene::addObject(Object *obj)
{
    objectList.push_back(obj);
    obj->linkedScene = this;
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
        delete obj;
    }
}

#pragma endregion
