#ifndef GENERIC_BULLET_H
#define GENERIC_BULLET_H

class RigidBodyComponent;
class SpriteComponent;

class genericBullet : public Object
{
public:
    genericBullet(Scene *scene, Vect newPos);
    ~genericBullet() override = default;
    void update() override;
    double aliveFor;
    Vect bulletSpeed;

private:
    RigidBodyComponent *rb;
    SpriteComponent *sp;
    Vect forceToApply;
};

#endif