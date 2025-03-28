#include "engine.h"
#include "player.h"
#include "rocketModules.h"
#define center {0.0, -10}
#define bulletSpeed {0.0, 350.0};

PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{

    addTag(TAG_PLAYER);
    // TODO: Create Layering system for rendering queue
    setName("PlayerObject");
    playerGrid = new Grid(this, {4, 3}, 100, "PlayerGrid");
    PlayerRB = new RigidBodyComponent(1, this);
    forceToApply = {0.0, 0.0};
    playerSpeed = 3000.0;
    move({(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2});
    PlayerRB->setEnergyLoss(0.03);
    iVect gridSize = playerGrid->getSize() * playerGrid->getCellSize();
    SDL_Rect box = {0, 0, gridSize.x, gridSize.y};
    centerRect(&box);
    PlayerRB->setCollision(&box);
    playerGrid->addObj(5, new defaultBlock(scene));
    playerGrid->addObj(6, new heatCore(scene));
    playerGrid->addObj(2, new cockpit2Block(scene));
    playerGrid->addObj(1, new gun1Block(scene));
    playerGrid->addObj(9, new engineBlock(scene));
    playerGrid->addObj(10, new engineBlock(scene));

    log(LOG_INFO)
        << "Created player object (" << this << ")" << std::endl;
}

void PlayerObject::update()
{
    playerGrid->move(pos);

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
    PlayerRB->applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}
