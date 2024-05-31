#include "targetObject.h"

targetObject::targetObject(Scene *scene, int number) : Object(scene)
{
    LOG_INIT_CERR();
    std::string newName = "TARGET " + std::to_string(number);
    this->setName(newName);
    log(LOG_INFO) << "Created target " << newName << "\n";
    // RigidBodyComponent *targetRB = new RigidBodyComponent;
    // this->addComponent(targetRB);
    // std::vector<iVect> box{{25, 25}, {-25, -25}}; // TODO: Dealocate
    // targetRB->setCollision(&box);
    // this->pos = {(double)(rand() % 500), (double)(rand() % 500)};
}
targetObject::~targetObject()
{
    this->destroy();
}

void targetObject::update()
{
    // TODO: Destroy target on impact
}
