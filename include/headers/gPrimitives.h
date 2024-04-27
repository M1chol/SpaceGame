#include "engine.h"

struct Vect{
    float x;
    float y;
};

struct iVect{
    int x;
    int y;
};

class Component{
};

iVect toiVect(Vect vect);

class gObject{

    public:
        gObject();

        ~gObject();

        void destroy();

        void addComponent(Component* comp);

        Component* getComponent(int componentId);

        Vect pos;

        std::vector<Component*> componentList;

    private:
        int nrOfComponents;

    
};