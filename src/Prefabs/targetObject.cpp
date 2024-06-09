#include "targetObject.h"

std::vector<iVect> box{{25, 25}, {-25, -25}};

targetObject::targetObject(Scene *scene, int number) : Object(scene)
{
    LOG_INIT_CERR();
    std::string newName = "TARGET " + std::to_string(number);
    this->setName(newName);
    log(LOG_INFO) << "Created target " << newName << " (" << this << ")\n";
    RigidBodyComponent *targetRB = new RigidBodyComponent;
    this->addComponent(targetRB);
    targetRB->setCollision(&box);
    this->pos = {(double)(rand() % 500), (double)(rand() % 500)};
}

void targetObject::update()
{
    if (this->getComponent<RigidBodyComponent>()->isColliding(mainScene->getObjectByName("PlayerObject")->getComponent<RigidBodyComponent>()))
    {
        delete this;
    }
}
