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
#pragma endregion

#pragma region Scene definitions

Scene::Scene()
{
    nrOfActiveObjects = 0;
    name = "Scene";
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
void Scene::setName(std::string newName){
    name = newName;
}
bool Scene::addObject(Object *obj)
{
    objectList.push_back(obj);
    nrOfActiveObjects++;
}

int Scene::Update()
{
    int temp = 0;
    for (auto &obj : objectList)
    {
        if (obj->isActive)
        {
            temp++;
            obj->render();
        }
    }
    return temp;
}

#pragma endregion
