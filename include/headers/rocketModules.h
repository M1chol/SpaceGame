#include "engine.h"

class heatCore : public Object
{
public:
    heatCore(Scene *scene);
};

class defaultBlock : public Object
{
public:
    defaultBlock(Scene *scene);
};

class engineBlock : public Object
{
public:
    engineBlock(Scene *scene);
};
class gun1Block : public Object
{
public:
    gun1Block(Scene *scene);
    void update() override;

private:
    SpriteComponent *base;
    SpriteComponent *gun;
    int index;
    double timer;
};
class cockpit2Block : public Object
{
public:
    cockpit2Block(Scene *scene);
};

class angleBlock : public Object
{
public:
    angleBlock(Scene *scene);
};