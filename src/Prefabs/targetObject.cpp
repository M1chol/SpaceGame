#include "targetObject.h"

SDL_Rect box{25, 25, 50, 50};

targetObject::targetObject(Scene *scene, int number) : Object(scene)
{

    std::string newName = "TARGET " + std::to_string(number);
    setName(newName);
    addTag(TAG_ENEMY);
    log(LOG_INFO) << "Created target " << newName << " (" << this << ")\n";
    rb = new RigidBodyComponent;
    addComponent(rb);
    rb->setCollision(&box);
    move({(double)(rand() % 500), (double)(rand() % 500)});
}

void targetObject::update()
{
    Object::update();
}

void targetObject::lateUpdate()
{
    RigidBodyComponent *targ = rb->isColliding(TAG_PLAYER);
    if (targ)
    {
        destroy();
    }
}
