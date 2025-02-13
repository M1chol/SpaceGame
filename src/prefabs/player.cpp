#include "engine.h"
#include "player.h"
#define center {0.0, -10}
#define bulletSpeed {0.0, 350.0};

PlayerObject::PlayerObject(Scene *scene) : Object(scene), playerGrid(scene, {4, 3}, 60)
{

    addTag(TAG_PLAYER);
    // TODO: Create Layering system for rendering queue
    setName("PlayerObject");
    addTag(TAG_PLAYER);
    PlayerRB = new RigidBodyComponent(1, this);
    bulletSpawner = new SpawnerComponent<genericBullet>(center, 0.2, 2);
    forceToApply = {0.0, 0.0};
    playerSpeed = 3000.0;
    move({(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2});
    PlayerRB->setEnergyLoss(0.03);
    iVect gridSize = playerGrid.getSize() * playerGrid.getCellSize();
    SDL_Rect box = {0, 0, gridSize.x, gridSize.y};
    centerRect(&box);
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
    playerGrid.move(pos);
}