#include "engine.h"

genericBullet::genericBullet(Scene *scene, Vect newPos) : Object(scene)
{
    rb = new RigidBodyComponent(0);
    sp = new SpriteComponent("res/bullet.png");
    setName("GenericBullet");
    addComponent(rb);
    addComponent(sp);
    sp->setScale(0.2);
    move(newPos);
    rb->applyForce({0.0, -1000.0});
    std::vector<iVect> box = {{10, 15}, {-10, -15}};
    rb->setCollision(&box);
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