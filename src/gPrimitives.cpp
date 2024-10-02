#include "engine.h"

#pragma region Vect

Vect Vect::operator*(double scalar)
{
    return Vect{x * scalar, y * scalar};
}
Vect Vect::operator+(const Vect &other)
{
    return Vect{x + other.x, y + other.y};
}
Vect Vect::operator-(const Vect &other)
{
    return Vect{x - other.x, y - other.y};
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
iVect Vect::toIVect()
{
    return iVect{(int)x, (int)y};
}

iVect iVect::operator+(const iVect &other)
{
    return iVect{x + other.x, y + other.y};
}
iVect iVect::operator*(int scalar)
{
    return iVect{x * scalar, y * scalar};
}

iVect &iVect::operator+=(const iVect &other)
{
    x += other.x;
    y += other.y;
    return *this;
}
iVect &iVect::operator*=(int scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

#pragma endregion

#pragma region Object

Object::Object(Scene *scene)
{
    // Set default position
    linkedScene = scene;
    linkedScene->addObject(this);
    pos = {0, 0};
    name = "unnamed";
    nrOfComponents = 0;
    isActive = true;
    posLocked = false;
}
Object::~Object()
{
    LOG_INIT_CERR();
    log(LOG_INFO) << "Destroying " << this->name << "\n";
    for (int i = nrOfComponents - 1; i >= 0; i--)
    {
        Component *component = componentList[i];
        log(LOG_INFO) << "destoyed component (" << component << ") in " << this->name << "\n";
        delete component;
        component = nullptr;
    }
    nrOfComponents = 0;
    componentList.clear();
    componentList.shrink_to_fit();
    if (!linkedScene->removeObject(this))
    {
        log(LOG_WARN) << "Could not remove " << name << " Object from linked scene\n";
    }
    linkedScene = nullptr;
    log(LOG_INFO) << "Removed object " << name << " (" << this << ")\n";
}
void Object::destroy()
{

    isActive = false;
    this->linkedScene->toBeRemoved.push_back(this);
}
Vect Object::getPos()
{
    return pos;
}
bool Object::move(Vect newPos, bool Forced)
{
    if (!Forced && posLocked)
    {
        return false;
    }
    pos = newPos;
    return true;
}
void Object::addComponent(Component *comp)
{
    // TODO: add check if component is already added
    componentList.push_back(comp);
    comp->setParent(this);
    comp->whenLinked();
    nrOfComponents++;
}
void Object::addTag(TAG newTag)
{
    linkedTags.push_back(newTag);
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
    if (showDebugNames)
    {
        new TextComponent(name, pos, globalFont, this);
    }
}
std::string Object::getName()
{
    return name;
}
bool Object::removeComponent(Component *comp)
{
    if (comp == nullptr)
    {
        return false;
    }
    auto el = std::find(componentList.begin(), componentList.end(), comp);
    if (el != componentList.end())
    {
        componentList.erase(el);
        nrOfComponents--;
        return true;
    }
    return false;
}
void Object::render()
{
    for (Component *component : componentList)
    {
        if (!component->render())
        {
            removeComponent(component);
            LOG_INIT_CERR();
            log(LOG_WARN) << "Removed faulty component (" << this << ") in " << this->getName() << "\n";
        }
    }
}
void Object::update()
{
    for (auto &component : componentList)
    {
        component->update();
    }
}
void Object::lateUpdate() {}
#pragma endregion

#pragma region Component

void Component::setParent(Object *new_parent)
{
    parent = new_parent;
}
bool Component::render() { return true; };
bool Component::update() { return true; };
Component::~Component()
{
    // this->destroy();
}
void Component::whenLinked() {};
Object *Component::getParent()
{
    return parent;
}

#pragma endregion

#pragma region Scene

Scene::Scene(SDL_Renderer *newRenderer)
{
    sceneList.push_back(this);
    nrOfObjects = 0;
    name = "unnamed";
    sceneRenderer = newRenderer;
}
Scene::~Scene()
{
    LOG_INIT_CERR();
    for (int i = nrOfObjects - 1; i > 0; i--)
    {
        objectList[i]->destroy();
    }
    objectList.clear();
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
    nrOfObjects++;
}
int Scene::Update()
{
    SDL_SetRenderDrawColor(sceneRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sceneRenderer);
    int updatedObjects = 0;
    // True Update
    for (int i = 0; i < nrOfObjects; i++)
    {
        Object *obj = objectList[i];
        if (obj->isActive)
        {
            updatedObjects++;
            obj->render();
            obj->update();
        }
    }
    handleCollisions();
    //  Late Update
    //  HACK: create list of objects for late update to optimize
    for (int i = 0; i < nrOfObjects; i++)
    {
        Object *obj = objectList[i];
        if (obj->isActive)
        {
            obj->lateUpdate();
        }
    }
    removeSheduled();
    SDL_RenderPresent(sceneRenderer);
    return updatedObjects;
}
SDL_Renderer *Scene::getRenderer()
{
    return sceneRenderer;
}
bool Scene::removeObject(Object *obj)
{
    LOG_INIT_CERR();
    auto el = std::find(objectList.begin(), objectList.end(), obj);
    if (el != objectList.end())
    {
        log(LOG_DEBUG) << "Removed " << obj->getName() << " from " << this->getName() << "\n";
        objectList.erase(el);
        nrOfObjects--;
        return true;
    }
    return false;
}
Object *Scene::getObjectByName(std::string name)
{
    for (auto &obj : objectList)
    {
        if (obj->getName() == name)
        {
            return obj;
        }
    }
    return nullptr;
}
std::vector<Object *> Scene::getObjectByTag(TAG tag)
{
    std::vector<Object *> objects;
    LOG_INIT_CERR();
    for (auto &obj : objectList)
    {
        for (auto &ltag : obj->linkedTags)
        {
            if (tag == ltag)
            {
                objects.push_back(obj);
            }
        }
    }
    return objects;
}
bool Scene::handleCollisions()
{
    for (int currObj = 0; currObj < nrOfObjects; currObj++)
    {
        RigidBodyComponent *rb1 = objectList[currObj]->getComponent<RigidBodyComponent>();
        if (rb1 == nullptr || !rb1->hasCollision)
        {
            return false;
        }
        iVect maxA = objectList[currObj]->getPos().toIVect() + rb1->getHitBox()[0];
        iVect minA = objectList[currObj]->getPos().toIVect() + rb1->getHitBox()[1];

        for (int i = currObj + 1; i < nrOfObjects; i++)
        {
            RigidBodyComponent *rb2 = objectList[i]->getComponent<RigidBodyComponent>();
            if (rb2 == nullptr || !rb2->hasCollision)
            {
                continue;
            }
            iVect maxB = objectList[i]->getPos().toIVect() + rb2->getHitBox()[0];
            iVect minB = objectList[i]->getPos().toIVect() + rb2->getHitBox()[1];
            double d1x = minB.x - maxA.x;
            double d1y = minB.y - maxA.y;
            double d2x = minA.x - maxB.x;
            double d2y = minA.y - maxB.y;
            if (d1x > 0.0 || d1y > 0.0)
            {
                continue;
            }
            if (d2x > 0.0 || d2y > 0.0)
            {
                continue;
            }
            rb2->solveCollision(rb1);
            rb1->solveCollision(rb2);
        }
    }
    return true;
}
void Scene::removeSheduled()
{
    for (auto *obj : toBeRemoved)
    {
        delete obj;
    }
    toBeRemoved.clear();
}

int Scene::getNrOfObjects()
{
    return objectList.size();
}

#pragma endregion
