#ifndef Layouts
#define CompLayoutsonents

#include "engine.h"

#pragma once

class Grid : public Object
{
public:
    Grid(Scene *scene, iVect size, double setCellSize);
    ~Grid();
    void addObj(iVect pos);
    void removeObj(iVect pos);

private:
    iVect size;
    double cellSize;
    std::vector<Object *> linkedObjects;
    bool isActive;
    bool draw;
};

class Family : public Object{
public:
    Family(Scene* scene, int familySize);
    ~Family();
    void addObj(Vect pos);
    void removeObject(int id);

private:
    int familySize;
    bool isActive;
};

#endif