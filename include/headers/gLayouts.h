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
    int ID;
    void saveBin(std::ofstream &out) override;
    void loadBin(std::ifstream &in) override;
};

class Grid : public Layout
{
public:
    Grid(Scene *scene, iVect size, double setCellSize);
    bool addObj(iVect loc, Object *obj);
    bool removeObj(int id, bool manual) override;
    void render() override;
    void update() override;
    void saveBin(std::ofstream &out);
    void loadBin(std::ifstream &in);

private:
    iVect size;
    Vect calculateSpaceCoordinates(iVect loc);
    int iVectToId(iVect location);
    iVect IdToIVect(int id);
    Vect gridCenter;
    double cellSize;
    std::vector<Object *> linkedObjects;
    std::vector<int> linkedObjectsId;
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