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
    // std::cout << mainScene->getObjectByName("PlayerObject")->getComponent<RigidBodyComponent>()->collisionList.size();
    RigidBodyComponent *targ = rb->isColliding(TAG_ENEMY, true);
    if (targ)
    {
        // std::cout << "TEST\n";
        targ->getParent()->destroy();
    }
}