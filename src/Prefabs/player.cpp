#include "engine.h"
#include "player.h"

double playerSpeed = 350.0;
RigidBodyComponent *PlayerRB = new RigidBodyComponent(10);
Vect center = {0.0, 0.0};
SpawnerComponent<genericBullet> *bulletSpawner = new SpawnerComponent<genericBullet>(center, 0.2, 2);
Vect forceToApply = {0.0, 0.0};
// double timer = 0.0;
Vect bulletSpeed = {0.0, 350.0};

PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    LOG_INIT_CERR();
    setName("PlayerObject");
    addComponent(bulletSpawner);
    addComponent(new SpriteComponent("res/player-placeholder.png"));
    addComponent(PlayerRB);
    pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    PlayerRB->setEnergyLoss(0.03);
    std::vector<iVect> box = {{66, 70}, {-66, -70}};
    PlayerRB->setCollision(&box);
    log(LOG_INFO) << "Created player object (" << this << ")\n";
}

void PlayerObject::update()
{
    Object::update();
    if (isKeyDown(SDL_SCANCODE_W))
    {
        forceToApply += {0.0, -1.0};
    }
    if (isKeyDown(SDL_SCANCODE_S))
    {
        forceToApply += {0.0, 1.0};
    }
    if (isKeyDown(SDL_SCANCODE_A))
    {
        forceToApply += {-1.0, 0.0};
    }
    if (isKeyDown(SDL_SCANCODE_D))
    {
        forceToApply += {1.0, 0};
    }
    if (isKeyDown(SDL_SCANCODE_Q))
    {
        remove();
        LOG_INIT_CERR();
        log(LOG_INFO) << "Oh no! player destroyed!\n";
    }
    if (isKeyDown(SDL_SCANCODE_SPACE))
    {
        bulletSpawner->shoot();
    }
    PlayerRB->applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}