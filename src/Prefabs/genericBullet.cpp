#include "engine.h"

genericBullet::genericBullet(Scene *scene, Vect newPos, Vect speed) : Object(scene)
{
    RigidBodyComponent *rb = new RigidBodyComponent(1.0);
    SpriteComponent *sp = new SpriteComponent("res/bullet.png");
    setName("GenericBullet");
    addComponent(rb);
    addComponent(sp);
    sp->setScale(0.5);
    pos = newPos;
    // rb.applyForce(speed);
}

void genericBullet::update()
{
    aliveFor += deltaTime;
}
