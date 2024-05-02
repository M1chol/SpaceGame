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

void gObject::addComponent(Component* comp){
    componentList.push_back(comp);
    nrOfComponents++;
    comp->setParent(this);
}

int gObject::render(){
    int count = 0;
    for(int i=0; i < nrOfComponents; i++){
        if(componentList[i]->render()){
            count++;
        }
    }
    return count;
}

#pragma endregion


void Component::setParent(gObject* new_parent){
    parent = new_parent;
}

bool Component::render(){
    return 0;
}