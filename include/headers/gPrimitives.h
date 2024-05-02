#include "engine.h"

struct Vect{
    float x;
    float y;
};

struct iVect{
    int x;
    int y;
};
class gObject;

class Component{
    public:
        void setParent(gObject* parent);
        virtual bool render();
    protected:
        gObject* parent;
};

iVect toiVect(Vect vect);

class gObject{

    public:
        gObject();

        ~gObject();

        void destroy();

        // goes through linked components and renders them
        int render();

        void addComponent(Component* comp);

        Component* getComponent(int componentId);

        Vect pos;
    private:
        int nrOfComponents;
        std::vector<Component*> componentList;

    
};