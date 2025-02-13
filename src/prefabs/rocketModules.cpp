#include "engine.h"

class heatCore : public Object
{
    heatCore(Scene *scene) : Object(scene)
    {
        setName("Heat Core");
        addComponent(new SpriteComponent("res/img/heatCore.png", this));
    }
};
class defaultBlock : public Object
{
    defaultBlock(Scene *scene) : Object(scene)
    {
        setName("Default Block");
        // TODO: Add system for spritesheets for now using different sprite
        addComponent(new SpriteComponent("res/img/storage.png", this));
    }
};
class engine : public Object
{
    engine(Scene *scene) : Object(scene)
    {
        setName("Default Block");
        // TODO: Add system for spritesheets for now using different sprite
        addComponent(new SpriteComponent("res/img/storage.png", this));
    }
};