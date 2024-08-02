#include "engine.h"

genericBullet::genericBullet(Scene *scene, Vect newPos) : Object(scene)
{
    rb = new RigidBodyComponent(0);
    sp = new SpriteComponent("res/bullet.png");
    setName("GenericBullet");
    addComponent(rb);
    addComponent(sp);
    sp->setScale(0.2);
    pos = newPos;
    rb->applyForce({0.0, -1000.0});
}

void genericBullet::update()
{
    Object::update();
    aliveFor += deltaTime;
}
