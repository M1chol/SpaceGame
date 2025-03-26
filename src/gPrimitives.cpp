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

Object::Object(Scene *scene, Object *parent)
{
    linkedScene = scene;
    if (!linkedScene)
    {
        log(LOG_WARN) << "Creating detached Object, supplied nullptr Scene\n";
    }
    else
    {
        linkedScene->addObject(this);
    }
    pos = {0, 0};
    name = "unnamed";
    nrOfComponents = 0;
    isActive = true;
    posLocked = false;
    rotation = 0;
    if (parent != nullptr)
    {
        parent->addChild(this);
    }
}

Object::Object(Object *parent){
    linkedScene = parent->getScene();
    pos = {0, 0};
    name = "unnamed";
    nrOfComponents = 0;
    isActive = true;
    posLocked = false;
    rotation = 0;
    this->parent = parent;
}

Object::~Object()
{
    log(LOG_INFO) << "Destroying " << this->name << "\n";
    for (int i = nrOfComponents - 1; i >= 0; i--)
    {
        Component *component = componentList[i];
        log(LOG_INFO) << " | destoyed " << component->getName() << " (" << component << ")\n";
        delete component;
    }
    componentList.clear();
    componentList.shrink_to_fit();
    for (int i = 0; i < childrenList.size(); i++)
    {
        delete childrenList[i];
    }
    if (!linkedScene->removeObject(this))
    {
        log(LOG_WARN) << "Could not remove " << name << " Object from linked scene\n";
    }
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
bool Object::addComponent(Component *comp)
{
    for (Component *cp : componentList)
    {
        if (comp == cp)
        {
            log(LOG_WARN) << "Object::addComponent of type " << comp->getName() << " failed " << cp << " already added to " << getName() << "\n";
            return false;
        }
    }
    componentList.push_back(comp);
    comp->setParent(this);
    comp->whenLinked();
    nrOfComponents++;
    return true;
}
void Object::addChild(Object *child)
{
    childrenList.push_back(child);
}

bool Object::removeChild(Object *child)
{
    auto el = std::find(childrenList.begin(), childrenList.end(), child);
    if (el != childrenList.end())
    {
        childrenList.erase(el);
        return true;
    }
    return false;
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
        new TextComponent(name, pos, "res/Pixellettersfull-BnJ5.ttf", this);
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
        for (int i = nrOfComponents; i > 0; i--)
        {
            if (componentList[i] == nullptr)
            {
                log(LOG_WARN) << "Removing faulty nullptr component from " << this->getName() << "\n";
                componentList.erase(componentList.begin() + i);
            }
        }
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
    for (auto &child : childrenList)
    {
        child->render();
    }

    for (Component *component : componentList)
    {
        if (!component->render())
        {
            removeComponent(component);

            log(LOG_WARN) << "Removed faulty component (" << this << ") in " << this->getName() << "\n";
        }
    }
}
void Object::update()
{
    for (auto &child : childrenList)
    {
        child->update();
    }

    for (auto &component : componentList)
    {
        if (component == nullptr)
        {
            removeComponent(component);
            break;
        }
        component->update();
    }
    for (auto &child : childrenList)
    {
        child->pos = pos;
    }
}
void Object::lateUpdate() {}
#pragma endregion

#pragma region Component

void Component::setParent(Object *new_parent)
{
    parent = new_parent;
}
bool Component::render() { return true; }
bool Component::update() { return true; }
Component::~Component()
{
    // this->destroy();
}
void Component::whenLinked() {};
Object *Component::getParent()
{
    return parent;
}
std::string Component::getName()
{
    if (name != "")
    {
        return name;
    }
    return "UNNAMED";
}

#pragma endregion

#pragma region Scene

Scene::Scene(std::string name)
{
    name = name;
    drawPriority = 0;
}
Scene::~Scene()
{
    for (int i = 0; i < objectList.size(); i++)
    {
        objectList[i]->destroy();
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
    if (obj == nullptr)
    {
        return false;
    }
    objectList.push_back(obj);
    return true;
}
int Scene::Update(bool skipPresent, bool skipClear)
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    if (!skipClear)
        SDL_RenderClear(gRenderer);
    int updatedObjects = 0;
    // True Update
    for (int i = 0; i < objectList.size(); i++)
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
    for (int i = 0; i < objectList.size(); i++)
    {
        Object *obj = objectList[i];
        if (obj->isActive)
        {
            obj->lateUpdate();
        }
    }
    removeSheduled();
    if (!skipPresent)
        SDL_RenderPresent(gRenderer);
    return updatedObjects;
}
SDL_Renderer *Scene::getRenderer()
{
    return gRenderer;
}
bool Scene::removeObject(Object *obj)
{

    auto el = std::find(objectList.begin(), objectList.end(), obj);
    if (el != objectList.end())
    {
        log(LOG_INFO) << "Removed " << obj->getName() << " from " << this->getName() << "\n";
        objectList.erase(el);
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
    int nrOfObjects = objectList.size();
    for (int currObj = 0; currObj < nrOfObjects; currObj++)
    {
        RigidBodyComponent *rb1 = objectList[currObj]->getComponent<RigidBodyComponent>();
        if (rb1 == nullptr || !rb1->hasCollision)
        {
            continue;
        }
        iVect maxA = objectList[currObj]->getPos().toIVect() + rb1->getHitBox(0);
        iVect minA = objectList[currObj]->getPos().toIVect() + rb1->getHitBox(1);

        for (int i = currObj + 1; i < nrOfObjects; i++)
        {
            RigidBodyComponent *rb2 = objectList[i]->getComponent<RigidBodyComponent>();
            if (rb2 == nullptr || !rb2->hasCollision)
            {
                continue;
            }
            iVect maxB = objectList[i]->getPos().toIVect() + rb2->getHitBox(0);
            iVect minB = objectList[i]->getPos().toIVect() + rb2->getHitBox(1);
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
    // nrOfObjects -= toBeRemoved.size();

    for (auto &obj : toBeRemoved)
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
