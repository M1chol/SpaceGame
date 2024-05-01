#include "engine.h"

iVect toiVect(Vect vect){
    return {(int)(vect.x + 0.5), (int)(vect.y+0.5)};
}

#pragma region gObject definitions
gObject::gObject(){
    // Set default position
    pos = {0, 0};
    nrOfComponents = 0;
}

gObject::~gObject(){
    for(int i=0; i<nrOfComponents; i++){
        delete componentList[i];
    }
}

Component* gObject::getComponent(int componentId){
    return componentList[componentId];
}

void Component::setParent(gObject* new_parent){
    parent = new_parent;
}

void gObject::addComponent(Component* comp){
    componentList.push_back(comp);
    comp->setParent(this);
}

#pragma endregion
