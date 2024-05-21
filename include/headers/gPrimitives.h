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
    virtual ~Component();
    void setParent(Object *parent);
    virtual void whenLinked();
    virtual bool render();
    virtual void destroy();

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
    void render();
    virtual void update();
    void addComponent(Component *comp);
    void removeComponent(Component *comp);
    Component *getComponent(int componentId);
    Vect pos;
    bool isActive;
    Scene *linkedScene;

private:
    //int nrOfComponents;
    std::vector<Component *> componentList;
};

class Scene
{
public:
    Scene(SDL_Renderer *);
    ~Scene();
    // Used to add object to objectList
    bool addObject(Object *);
    bool removeObject(Object *);
    void destroy();
    // Updates all active game Objects eg. rendering
    int Update();
    // Change name of scene
    void setName(std::string);
    // Returns pointer to renderer linked to Scene
    SDL_Renderer *getRenderer();

private:
    SDL_Renderer *sceneRenderer;
    std::vector<Object *> objectList;
    int nrOfActiveObjects;
    std::string name;
};