#ifndef G_PRIMITIVES_H
#define G_PRIMITIVES_H

#include "engine.h"

struct iVect;

/* Primitive struct that holds to floats x and y*/
struct Vect
{
    double x;
    double y;

    Vect operator+(const Vect &other);
    Vect operator-(const Vect &other);
    Vect operator*(double scalar);
    Vect &operator+=(const Vect &other);
    Vect &operator*=(double scalar);
    double magnitude();
    Vect normalized();
    iVect toIVect();
};
/* Primitive struct that holds to ints x and y*/
struct iVect
{
    int x;
    int y;

    iVect operator+(const iVect &other);
    iVect operator*(int scalar);
    iVect &operator+=(const iVect &other);
    iVect &operator*=(int scalar);
};

class Object;
class Scene;

class Component
{
public:
    virtual ~Component() = 0;
    /* Sets parent variable of Component class @param parent parent object*/
    void setParent(Object *parent);
    /* Executes when Component is linked to Object*/
    virtual void whenLinked();
    /* Call render function of child object if exists overload*/
    virtual bool render();
    /* Call update function of child object */
    virtual bool update();
    virtual void saveBin(std::ofstream &out);
    virtual void loadBin(std::ifstream &in);
    Object *getParent();
    std::string getName();

protected:
    Object *parent;
    std::string name;
};

class Object
{

public:
    /* Creates object linked to a scene @param scene Object will be updated and rendered by this scene @param parent sets the parent*/
    Object(Scene *scene, Object *parent = nullptr);
    /* Creates Object that is updated and rendered by its parent @param parent*/
    Object(Object *parent);
    virtual ~Object();

    // virtual void destroy();

    /* goes through linked components and renders them if they support it */
    virtual void render();
    /* goes through linked components and updates them if they support it */
    virtual void update();
    /* Runs after solving collisions */
    virtual void lateUpdate();
    /* Saving to binary for Object type */
    virtual void saveBin(std::ofstream &out);
    /* Loading to binary for Object type */
    virtual void loadBin(std::ifstream &in);
    // Handles scheduling of objects for deconstruction
    virtual void destroy();
    /* add Component to vector componentList of Object @param comp Component to be linked */
    bool addComponent(Component *comp);
    /* add Child to childList @param child pointer to child*/
    void addChild(Object *child);
    /* add Tag to existing `Object` @param newTag tag to be added*/
    void addTag(TAG newTag);
    /* remove Component from vector componentList of Object @param comp Component to be removed
    @return `true` if successfull and `false` if component was not found */
    bool removeComponent(Component *comp);
    bool removeChild(Object *child);
    /* Get pointer to component of a specified type */
    template <typename CompType>
    CompType *getComponent()
    {
        for (Component *comp : componentList)
        {
            if (CompType *specificComp = dynamic_cast<CompType *>(comp))
            {
                return specificComp;
            }
        }
        return nullptr;
    }
    /* Set linked scene variable of Object @param scene pointer to scene */
    void setScene(Scene *scene);
    /* Get Scene pointer of linked scene @return pointer to linked scene*/
    Scene *getScene();
    void setName(std::string newName);
    std::string getName();
    std::vector<Object *> *getChildrenList() { return &childrenList; };
    /*Move `Object` to `Scene` coordinets @param newPos position to be moved to @param Forced use only when `Object` should ignore `posLocked` state*/
    bool move(Vect newPos, bool Forced = false);
    void rotate(double angle) { rotation = angle; };
    double getRotation() { return rotation; };
    /*Returns `Scene` coordinates of an Object*/
    Vect getPos();
    bool isActive;
    std::vector<TAG> linkedTags;
    /*If true `Object` will ignore move commands*/
    bool posLocked;

protected:
    Vect pos;
    double rotation;
    std::string name;

private:
    Scene *linkedScene;
    std::vector<Component *> componentList;
    std::vector<Object *> childrenList;
    int nrOfComponents;
    Object *parent;
};

class Scene
{
public:
    /* Crete new `Scene` with linked `SDL_Renderer` */
    Scene(std::string name = "UNNAMED");
    /* Destroy `Scene` and all linked `Objects` */
    ~Scene();
    /* add object to objectList */
    bool addObject(Object *obj);
    /* remove object from objectList*/
    bool removeObject(Object *obj);
    // void destroy();
    //  Updates all active game `Objects`
    int Update(bool skipPresent = false, bool skopClear = false);
    // Change name of scene
    void setName(std::string);
    std::string getName();
    // Returns pointer to renderer linked to Scene
    SDL_Renderer *getRenderer();
    bool handleCollisions();
    Object *getObjectByName(std::string name);
    std::vector<Object *> getObjectByTag(TAG);
    std::vector<Object *> toBeRemoved;
    // Remove items scheduled for destruction
    void removeSheduled();
    int getNrOfObjects();
    void setDrawPriority(int newPriority) { drawPriority = newPriority; };
    int getDrawPriority() { return drawPriority; };

private:
    std::vector<Object *> objectList;
    std::string name;
    int drawPriority;
};

#endif