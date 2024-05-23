#include "engine.h"
#include "player.h"

int playerSpeed = 2;

PlayerObject::PlayerObject()
{
    LOG_INIT_CERR();
    log(LOG_INFO) << "Creating player object\n";
    mainScene->addObject(this);
    this->addComponent(new SpriteComponent("res/player-placeholder.png"));
    this->pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
}

PlayerObject::~PlayerObject()
{
    this->destroy();
}

void PlayerObject::update()
{
    if (isKeyDown(SDL_SCANCODE_W))
    {
        this->pos.y -= playerSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_S))
    {
        this->pos.y += playerSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_A))
    {
        this->pos.x -= playerSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_D))
    {
        this->pos.x += playerSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_Q))
    {
        this->destroy();
        LOG_INIT_CERR();
        log(LOG_INFO) << "Oh no! player destroyed!\n";
    }
}