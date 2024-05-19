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

class Component
{
public:
    void setParent(Object *parent);
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

private:
    int nrOfComponents;
    std::vector<Component *> componentList;
};

class Scene
{
    // TODO: Finish implementation
public:
    Scene();
    ~Scene();
    bool addObject(Object *);
    void destroy();
    int Update();
    void setName(std::string);

private:
    std::vector<Object *> objectList;
    int nrOfActiveObjects;
    std::string name;
};