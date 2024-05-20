#include "engine.h"

// iVect toiVect(Vect vect){
//     return {(int)(vect.x + 0.5), (int)(vect.y+0.5)};
// }

#pragma region gObject definitions

Object::Object()
{
    // Set default position
    pos = {0, 0};
    nrOfComponents = 0;
    linkedScene = nullptr;
}
Object::~Object()
{
    destroy();
}
void Object::destroy()
{
    for (int i = 0; i < nrOfComponents; i++)
    {
        delete componentList[i];
    }
}

Component *Object::getComponent(int componentId)
{
    return componentList[componentId];
}

void Object::addComponent(Component *comp)
{
    componentList.push_back(comp);
    nrOfComponents++;
    comp->setParent(this);
    comp->whenLinked();
}

int Object::render()
{
    int count = 0;
    for (int i = 0; i < nrOfComponents; i++)
    {
        if (componentList[i]->render())
        {
            count++;
        }
    }
    return count;
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
        object->destroy();
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

#pragma endregion
