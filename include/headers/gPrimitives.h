#include "engine.h"

struct iVect;

/* Primitive struct that holds to floats x and y*/
struct Vect
{
    double x;
    double y;

    Vect operator+(const Vect &other);
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
    virtual ~Component();
    /* Sets parent variable of Component class @param parent parent object*/
    void setParent(Object *parent);
    /* Executes when Component is linked to Object*/
    virtual void whenLinked();
    /* Call render function of child object if exists overload*/
    virtual bool render();
    /* Call update function of child object */
    virtual bool update();
    /* Call destroy function of child object*/

protected:
    Object *parent;
};

class Object
{

public:
    Object(Scene *scene);

    virtual ~Object();

    /* goes through linked components and renders them if they support it */
    void render();
    /* goes through linked components and updates them if they support it */
    virtual void update();
    /* add Component to vector componentList of Object @param comp Component to be linked */
    void addComponent(Component *comp);
    /* remove Component from vector componentList of Object @param comp Component to be removed
    @return `true` if successfull and `false` if component was not found */
    bool removeComponent(Component *comp);
    /* Get pointer to component at specified index @param componentID id of component @return pointer to component */
    template <typename CompType>
    CompType *getComponent(); // TODO: Remove componentId, search based on component type using tamplates
    /* Set linked scene variable of Object @param scene pointer to scene */
    void setScene(Scene *scene);
    /* Get Scene pointer of linked scene @return pointer to linked scene*/
    Scene *getScene();
    void setName(std::string newName);
    std::string getName();
    Vect pos;
    bool isActive;

private:
    Scene *linkedScene;
    std::vector<Component *> componentList;
    std::string name;
    int nrOfComponents;
};

class Scene
{
public:
    /* Crete new `Scene` with linked `SDL_Renderer` */
    Scene(SDL_Renderer *rend);
    ~Scene();
    /* add object to objectList */
    bool addObject(Object *obj);
    /* remove object from objectList*/
    bool removeObject(Object *obj);
    // Updates all active game `Objects`
    int Update();
    // Change name of scene
    void setName(std::string);
    std::string getName();
    // Returns pointer to renderer linked to Scene
    SDL_Renderer *getRenderer();
    bool solveCollisions(int objectNr);

private:
    SDL_Renderer *sceneRenderer;
    std::vector<Object *> objectList;
    int nrOfObjects;
    std::string name;
};