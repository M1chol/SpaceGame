#include "engine.h"

struct Vect
{
    float x;
    float y;
};

struct iVect
{
    int x;
    int y;
};

class Object;
class Scene;

class Component
{
public:
    void setParent(Object *parent);
    virtual void whenLinked();
    virtual bool render();

protected:
    Object *parent;
};

class Object
{

public:
    Object();

    ~Object();

    void destroy();

    // goes through linked components and renders them
    int render();
    void addComponent(Component *comp);
    Component *getComponent(int componentId);
    Vect pos;
    bool isActive;
    Scene *linkedScene;

private:
    int nrOfComponents;
    std::vector<Component *> componentList;
};

class Scene
{
    // TODO: Finish implementation
public:
    Scene(SDL_Renderer*);
    ~Scene();
    bool addObject(Object *);
    void destroy();
    int Update();
    void setName(std::string);
    SDL_Renderer *getRenderer();

private:
    SDL_Renderer *sceneRenderer;
    std::vector<Object *> objectList;
    int nrOfActiveObjects;
    std::string name;
};