#include "rocketModules.h"

heatCore::heatCore(Scene *scene) : Object(scene)
{
    setName("Heat Core");
    SpriteComponent *t = new SpriteComponent("res/img/heatCore.png", this);
}

defaultBlock::defaultBlock(Scene *scene) : Object(scene)
{
    setName("Default Block");
    // TODO: Add system for spritesheets for now using different sprite
    SpriteComponent *t = new SpriteComponent("res/img/storage.png", this);
}

engineBlock::engineBlock(Scene *scene) : Object(scene)
{
    setName("Engine block");
    // TODO: Add system for spritesheets for now using different sprite
    SpriteComponent *t = new SpriteComponent("res/img/engine2.png", this);
}

gun1Block::gun1Block(Scene *scene) : Object(scene)
{
    setName("Gun 1");
    // TODO: Add system for spritesheets for now using different sprite
    SpriteComponent *t = new SpriteComponent("res/img/gun_base.png", this);
}

cockpit2Block::cockpit2Block(Scene *scene) : Object(scene)
{
    setName("Cockpit 2");
    // TODO: Add system for spritesheets for now using different sprite
    SpriteComponent *t = new SpriteComponent("res/img/cockpit2.png", this);
}

angleBlock::angleBlock(Scene *scene) : Object(scene)
{
    setName("Angled block");
    // TODO: Add system for spritesheets for now using different sprite
    SpriteComponent *t = new SpriteComponent("res/img/angle.png", this);
}
