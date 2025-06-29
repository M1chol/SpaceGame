#ifndef G_PRIMITIVES_H
#define G_PRIMITIVES_H

#include "engine.h"

struct iVect;

/* Primitive struct that holds to floats x and y*/
struct Vect
{
    double x = 0.0;
    double y = 0.0;
    Vect(double x_val, double y_val) : x(x_val), y(y_val) {}
    Vect() = default;
    Vect operator+(const Vect &other) const;
    Vect operator-(const Vect &other) const;
    Vect operator*(double scalar) const;
    Vect &operator+=(const Vect &other);
    Vect &operator*=(double scalar);
    double magnitude() const;
    Vect normalized() const;
    Vect rotate(double angle, Vect center) const;
    iVect toIVect() const;
};
/* Primitive struct that holds to ints x and y*/
struct iVect
{
    int x = 0;
    int y = 0;
    iVect(int x_val, int y_val) : x(x_val), y(y_val) {}
    iVect() = default;
    iVect operator+(const iVect &other) const;
    iVect operator*(int scalar) const;
    iVect &operator+=(const iVect &other);
    iVect &operator*=(int scalar);
    iVect &operator/=(int scalar);
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
    Object(Scene *scene);

    virtual ~Object();

    // virtual void destroy();

    /* goes through linked components and renders them if they support it */
    virtual void render();
    /* goes through linked components and updates them if they support it */
    virtual void update();
    /* Runs after solving collisions */
    virtual void lateUpdate();
    // Handles scheduling of objects for deconstruction
    virtual void destroy();
    /* add Component to vector componentList of Object @param comp Component to be linked */
    bool addComponent(Component *comp);
    /* add Tag to existing `Object` @param newTag tag to be added*/
    void addTag(TAG newTag);
    /* remove Component from vector componentList of Object @param comp Component to be removed
    @return `true` if successfull and `false` if component was not found */
    bool removeComponent(Component *comp);

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
    /*Move `Object` to `Scene` coordinets @param newPos position to be moved to @param Forced use only when `Object` should ignore `posLocked` state*/
    bool moveAdv(Vect newPos, bool Forced = false);
    bool move(Vect newPos) { moveAdv(newPos, false); };
    void rotate(double angle) { rotation = angle; };
    double getRotation() { return rotation; };
    /*Returns `Scene` coordinates of an Object*/
    Vect getPos() { return pos; };
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
    int nrOfComponents;
};

class Scene
{
public:
    /* Crete new `Scene` with linked `SDL_Renderer` */
    Scene(std::string name = "UNNAMED");
    /* Destroy `Scene` and all linked `Objects` */
    virtual ~Scene();
    /* add object to objectList */
    bool addObject(Object *obj);
    /* remove object from objectList*/
    bool removeObject(Object *obj);
    // void destroy();
    //  Updates all active game `Objects`
    int Update(bool skipPresent = false, bool skopClear = false);
    virtual void update() {};
    // Change name of scen
    void setName(std::string);
    std::string getName();
    // Returns pointer to renderer linked to Scene
    SDL_Renderer *getRenderer();
    bool handleCollisions();
    Object *getObjectByName(std::string name);
    std::vector<Object *> getObjectsByTag(TAG);
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