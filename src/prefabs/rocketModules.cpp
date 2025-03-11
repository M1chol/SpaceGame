#include "rocketModules.h"

heatCore::heatCore(Scene *scene) : Object(scene)
{
    setName("Heat Core");
    SpriteComponent *t = new SpriteComponent("res/img/heatCore.png", this);
}

defaultBlock::defaultBlock(Scene *scene) : Object(scene)
{
    setName("Default Block");
    SpriteComponent *t = new SpriteComponent("res/img/storage.png", this);
}

engineBlock::engineBlock(Scene *scene) : Object(scene)
{
    setName("Engine block");
    SpriteComponent *t = new SpriteComponent("res/img/engine2.png", this);
}

gun1Block::gun1Block(Scene *scene) : Object(scene)
{
    setName("Gun 1");
    base = new SpriteComponent("res/img/gun_base.png", this);
    gun = new SpriteComponent("res/img/gun_s.png", this);
    shooter = new SpawnerComponent<genericBullet>(this, {0, 0}, 0.3, 5);
    gun->setSheetIndex(1);
}
void gun1Block::update()
{
    double angle = atan2(mousePos.y - pos.y, mousePos.x - pos.x) * 180 / M_PI + 90;
    gun->rotate(angle);
    if (isKeyDown(MOUSE_LEFT))
    {
        shooter->shoot(angle);
    }
    Object::update();
}

cockpit2Block::cockpit2Block(Scene *scene) : Object(scene)
{
    setName("Cockpit 2");
    SpriteComponent *t = new SpriteComponent("res/img/cockpit2.png", this);
}

angleBlock::angleBlock(Scene *scene) : Object(scene)
{
    setName("Angled block");
    SpriteComponent *t = new SpriteComponent("res/img/angle.png", this);
}
