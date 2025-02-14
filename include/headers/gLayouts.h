#ifndef Layouts
#define CompLayoutsonents

#include "engine.h"

#pragma once
class Layout : public Object
{
public:
    Layout(Scene *scene, std::string name, Object *parent = nullptr);
    ~Layout() override;
    void destroy() override;
    virtual bool addObj();
    virtual bool removeObj(int id, bool manual);
    void saveBin(std::ofstream &out) override;
    void loadBin(std::ifstream &in) override;
    iVect getSize();
    int getID() { return ID; };

protected:
    iVect size;
    std::vector<Object *> linkedObjects;
    int ID;
};

class Grid : public Layout
{
public:
    Grid(Scene *scene, iVect size = {0, 0}, double setCellSize = 1, std ::string name = "Unnamed Grid ", Object *parent = nullptr);
    bool addObj(iVect loc, Object *obj);
    bool addObj(int id, Object *obj);
    bool removeObj(int id, bool manual) override;
    void render() override;
    void update() override;
    void saveBin(std::ofstream &out) override;
    void loadBin(std::ifstream &in) override;
    int getCellSize();

private:
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
    Family(Scene *scene, std::string name = "Unnamed Family ");
    void update() override;
    bool addObj(Object *obj, Vect objPos);
    bool removeObj(int id, bool manual) override;

private:
    int familySize;
    std::vector<Vect> lookupOffset;
};

#endif