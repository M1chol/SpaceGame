#include "engine.h"

class genericBullet : public Object
{
public:
    genericBullet(Scene *scene, Vect newPos, Vect speed);
    ~genericBullet() override = default;
    void update() override;
    double aliveFor;
};