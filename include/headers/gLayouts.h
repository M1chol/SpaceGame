#ifndef Layouts
#define CompLayoutsonents

#include "engine.h"

#pragma once
class Layout : public Object
{
public:
    Layout(Scene *scene);
    void destroy() override;
    virtual bool addObj();
    virtual bool removeObj(int id, bool manual);
    int ID;
    void saveBin(std::ofstream &out) override;
    void loadBin(std::ifstream &in) override;

protected:
    std::vector<Object *> linkedObjects;
};

class Grid : public Layout
{
public:
    Grid(Scene *scene, iVect size = {0, 0}, double setCellSize = 1);
    bool addObj(iVect loc, Object *obj);
    bool addObj(int id, Object *obj);
    bool removeObj(int id, bool manual) override;
    void render() override;
    void update() override;
    void saveBin(std::ofstream &out);
    void loadBin(std::ifstream &in);

private:
    iVect size;
    Vect calculateSpaceCoordinates(iVect loc);
    Vect calculateGridCenter();
    int iVectToId(iVect location);
    iVect idToIVect(int id);
    Vect gridCenter;
    double cellSize;
    std::vector<int> linkedObjectsId;
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
    std::vector<Vect> lookupOffset;
};

#endif