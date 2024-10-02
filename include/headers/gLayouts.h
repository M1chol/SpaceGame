#ifndef Layouts
#define CompLayoutsonents

#include "engine.h"

#pragma once
class Layout : public Object
{
public:
    Layout(Scene *scene);
    virtual ~Layout();
    virtual bool addObj();
    virtual bool removeObj(int id, bool manual);
};

class Grid : public Layout
{
public:
    Grid(Scene *scene, iVect size, double setCellSize);
    bool addObj(iVect loc, Object *obj);
    bool removeObj(int id, bool manual) override;
    void render() override;
    void update() override;

private:
    iVect size;
    Vect calculateSpaceCoordinates(iVect loc);
    Vect gridCenter;
    double cellSize;
    std::vector<std::vector<Object *>> linkedObjects;
    bool draw;
    SDL_Renderer *renderer;
};

class Family : public Layout
{
public:
    Family(Scene *scene);
    void update() override;
    bool addObj(Object *obj, Vect objPos);
    bool removeObj(int id, bool manual) override;

private:
    int familySize;
    std::vector<Object *> linkedObjects;
    std::vector<Vect> lookupOffset;
};

#endif