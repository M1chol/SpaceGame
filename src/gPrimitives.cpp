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

#pragma region Object definitions

Object::Object(Scene *scene)
{
    // Set default position
    linkedScene = scene;
    linkedScene->addObject(this);
    pos = {0, 0};
    name = "unnamed";
    nrOfComponents = 0;
    isActive = true;
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
    log(LOG_INFO) << "Removed object " << this->name << " (" << this << ")\n";
}
void Object::destroy()
{

    isActive = false;
    this->linkedScene->toBeRemoved.push_back(this);

}
void Object::addComponent(Component *comp)
{
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
        nrOfComponents--;
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
template <typename CompType>
CompType *Object::getComponent()
{
    LOG_INIT_CERR();
    if (this == nullptr)
    {
        return nullptr;
    }
    for (Component *comp : componentList)
    {
        if (CompType *specificComp = dynamic_cast<CompType *>(comp))
        {
            return specificComp;
        }
    }
    log(LOG_WARN) << "getComponent returned nullptr, this is not normal behaviour\n";
    return nullptr;
}
void Object::lateUpdate() {}
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
    // this->destroy();
}
void Component::whenLinked() {};
Object *Component::getParent()
{
    return parent;
}

#pragma endregion

#pragma region Scene definitions

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
    int temp = 0;
    // True Update
    for (int i = 0; i < nrOfObjects; i++)
    {
        Object *obj = objectList[i];
        if (obj->isActive)
        {
            //HACK: Calling the same 2 for loops. posisible fix here
            temp++;
            obj->render();
            obj->update();
            handleCollisions(i);
        }
    }
    // Late Update
    // HACK: create list of objects for late update to optimize
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
    return temp;
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
bool Scene::handleCollisions(int currObj)
{
    RigidBodyComponent *currRB = objectList[currObj]->getComponent<RigidBodyComponent>();
    if (currRB == nullptr)
    {
        return false;
    }
    if (!currRB->hasCollision)
    {
        return false;
    }
    iVect maxA = objectList[currObj]->pos.toIVect() + currRB->getHitBox()[0];
    iVect minA = objectList[currObj]->pos.toIVect() + currRB->getHitBox()[1];
    for (int j = currObj + 1; j < nrOfObjects; j++)
    {
        RigidBodyComponent *testRB = objectList[j]->getComponent<RigidBodyComponent>();
        if (testRB == nullptr || !testRB->hasCollision)
        {
            continue;
        }
        iVect maxB = objectList[j]->pos.toIVect() + testRB->getHitBox()[0];
        iVect minB = objectList[j]->pos.toIVect() + testRB->getHitBox()[1];
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
        currRB->solveCollision(testRB);
        testRB->solveCollision(currRB);
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

#pragma endregion
