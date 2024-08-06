#include "targetObject.h"

std::vector<iVect> box{{25, 25}, {-25, -25}};

targetObject::targetObject(Scene *scene, int number) : Object(scene)
{
    LOG_INIT_CERR();
    std::string newName = "TARGET " + std::to_string(number);
    setName(newName);
    addTag(TAG_ENEMY);
    log(LOG_INFO) << "Created target " << newName << " (" << this << ")\n";
    rb = new RigidBodyComponent;
    addComponent(rb);
    rb->setCollision(&box);
    pos = {(double)(rand() % 500), (double)(rand() % 500)};
}

void targetObject::update()
{
    Object::update();
}

void targetObject::lateUpdate()
{
    // std::cout << getComponent<RigidBodyComponent>()->collisionList.size();
    RigidBodyComponent *targ = rb->isColliding(TAG_PLAYER);
    // std::cout << targ;
    if (targ)
    {
        destroy();
    }
}
