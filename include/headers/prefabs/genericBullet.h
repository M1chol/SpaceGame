#ifndef GENERIC_BULLET_H
#define GENERIC_BULLET_H

class RigidBodyComponent;
class SpriteComponent;

class genericBullet : public Object
{
public:
    genericBullet(Scene *scene, Vect newPos);
    ~genericBullet() override = default;
    void reset(double angle, int speed, Vect position);
    void update() override;
    void lateUpdate() override;
    double aliveFor;
    Vect bulletSpeed;

private:
    RigidBodyComponent *rb;
    SpriteComponent *sp;
    Vect forceToApply;
};

#endif