#include "engine.h"
#include "player.h"
#define center {0.0, -10}
#define bulletSpeed {0.0, 350.0};

PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{

    addTag(TAG_PLAYER);
    // TODO: Create Layering system for rendering queue
    setName("PlayerObject");
    addComponent(new SpriteComponent("res/player-placeholder.png"));
    addTag(TAG_PLAYER);
    PlayerRB = new RigidBodyComponent(1, this);
    bulletSpawner = new SpawnerComponent<genericBullet>(center, 0.2, 2);
    forceToApply = {0.0, 0.0};
    playerSpeed = 3000.0;
    addComponent(bulletSpawner);
    move({(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2});
    PlayerRB->setEnergyLoss(0.03);
    SDL_Rect box = {66, 70, 132, 140};
    PlayerRB->setCollision(&box);
    log(LOG_INFO) << "Created player object (" << this << ")" << std::endl;
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
        destroy();

        log(LOG_INFO) << "Oh no! player destroyed!\n";
    }
    if (isKeyDown(SDL_SCANCODE_SPACE))
    {
        bulletSpawner->shoot();
    }
    PlayerRB->applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}