#include "engine.h"
#include "player.h"

double playerSpeed = 350.0;
RigidBodyComponent PlayerRB = RigidBodyComponent(10);
Vect forceToApply = {0.0, 0.0};

PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    LOG_INIT_CERR();
    this->setName("PlayerObject");
    this->addComponent(new SpriteComponent("res/player-placeholder.png"));
    this->addComponent(&PlayerRB);
    this->pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    PlayerRB.setEnergyLoss(0.03);
    std::vector<iVect> box = {{66, 70}, {-66, -70}}; // TODO: Dealocate
    PlayerRB.setCollision(&box);
    log(LOG_INFO) << "Created player object (" << this << ")\n";
}

PlayerObject::~PlayerObject()
{
    this->destroy();
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
        this->destroy();
        LOG_INIT_CERR();
        log(LOG_INFO) << "Oh no! player destroyed!\n";
    }
    PlayerRB.applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}