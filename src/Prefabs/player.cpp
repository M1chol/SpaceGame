#include "engine.h"
#include "player.h"

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
    destroy();
}
