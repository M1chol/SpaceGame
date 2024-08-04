#ifndef PLAYER_H
#define PLAYER_H

class RigidBodyComponent;
class SpriteComponent;

class PlayerObject : public Object
{
public:
    PlayerObject(Scene *scene);
    void lateUpdate() override;
    void update() override;

private:
    SDL_Event b;
    double playerSpeed;
    Vect forceToApply;
    RigidBodyComponent *PlayerRB;
    SpawnerComponent<genericBullet> *bulletSpawner;
};

#endif