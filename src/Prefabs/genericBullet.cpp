#include "engine.h"

genericBullet::genericBullet(Scene *scene, Vect newPos) : Object(scene)
{
    setName("GenericBullet");
    rb = new RigidBodyComponent(0, this);
    sp = new SpriteComponent("res/bullet.png", this);
    sp->setScale(0.2);
    move(newPos);
    rb->applyForce({0.0, -1000.0});
    std::vector<iVect> box = {{10, 15}, {-10, -15}};
    rb->setCollision(&box);
    aliveFor = 0;
}

void genericBullet::update()
{
    Object::update();
    aliveFor += deltaTime;
}

void genericBullet::lateUpdate()
{
    RigidBodyComponent *targ = rb->isColliding(TAG_ENEMY);
    if (targ)
    {
        targ->getParent()->destroy();
    }
}