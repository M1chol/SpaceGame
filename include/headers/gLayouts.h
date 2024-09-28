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
    virtual bool removeObj(int id);
};

class Grid : public Layout
{
public:
    Grid(Scene *scene, iVect size, double setCellSize);
    bool addObj(iVect loc, Object *obj);
    bool removeObj(int id) override;
    void render() override;
    void update() override;

private:
    iVect size;
    Vect calculateSpaceCoordinates(iVect loc);
    Vect gridCenter;
    double cellSize;
    std::vector<std::vector<Object *>> linkedObjects;
    bool isActive;
    bool draw;
    SDL_Renderer *renderer;
};

class Family : public Layout
{
public:
    Family(Scene *scene, int familySize);
    ~Family();
    bool addObj() override;
    bool addObj(Vect pos);
    bool removeObj(int id) override;

private:
    int familySize;
    bool isActive;
};

#endif