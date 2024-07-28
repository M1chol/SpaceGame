#include "engine.h"

class PlayerObject : public Object
{
public:
    PlayerObject(Scene *scene);
    void update() override;

private:
    SDL_Event b;
};