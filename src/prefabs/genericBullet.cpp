#include "engine.h"

genericBullet::genericBullet(Scene *scene, Vect newPos) : Object(scene)
{
    addTag(TAG_PLAYER);
    setName("GenericBullet");
    rb = new RigidBodyComponent(0, this);
    sp = new SpriteComponent("res/bullet.png", this);
    sp->setScale(0.2);
    move(newPos);
    SDL_Rect box = {10, 10, 20, 20};
    rb->setCollision(&box);
    aliveFor = 0;
}

void genericBullet::genericBullet::reset(double angle, int speed, Vect position)
{
    rotation = angle;
    isActive = true;
    move(position);
    double radians = rotation * M_PI / 180.0;
    rb->applyForce({sin(radians) * speed, -cos(radians) * speed});
}

void genericBullet::update()
{
    Object::update();
    aliveFor += deltaTime;
}

void genericBullet::lateUpdate()
{
    // RigidBodyComponent *targ = rb->isColliding(TAG_ENEMY);
    // if (targ)
    // {
    //     targ->getParent()->destroy();
    // }
}