#include "engine.h"

Layout::Layout(Scene *scene) : Object(scene) {}
Layout::~Layout() {};
bool Layout::addObj() {};
bool Layout::removeObj(int id, bool manual = true) {};

Grid::Grid(Scene *scene, iVect setSize, double setCellSize) : Layout(scene)
{

    size = setSize;
    setName("Grid Object");
    cellSize = setCellSize;
    renderer = scene->getRenderer();
    gridCenter = {size.x * cellSize / 2, size.y * (double)cellSize / 2};
    linkedObjects = std::vector<std::vector<Object *>>(
        size.x,
        std::vector<Object *>(size.y, nullptr));
    log(LOG_INFO) << "Created Grid\n";
}

void Grid::render()
{
    Object::render();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    Vect offsetPos = pos - gridCenter;
    double drawWidth = (int)(offsetPos.x + size.x * cellSize);
    double drawHeight = (int)(offsetPos.y + size.y * cellSize);
    for (int i = 0; i < size.y + 1; i++)
    {
        SDL_RenderDrawLine(renderer, (int)offsetPos.x, (int)offsetPos.y + cellSize * i, drawWidth, (int)offsetPos.y + cellSize * i);
    }
    for (int i = 0; i < size.x + 1; i++)
    {
        SDL_RenderDrawLine(renderer, (int)offsetPos.x + cellSize * i, (int)offsetPos.y, (int)offsetPos.x + cellSize * i, drawHeight);
    }
}

bool Grid::addObj(iVect loc, Object *obj)
{

    if (loc.x > size.x - 1 || loc.y > size.y - 1)
    {
        log(LOG_WARN) << "Grid::addObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    if (linkedObjects[loc.x][loc.y] != nullptr)
    {
        log(LOG_WARN) << "Grid::addObj err, space is already occupied\n";
        return false;
    }
    if (obj->getComponent<LayoutHelperComponent>() != nullptr)
    {
        log(LOG_WARN) << "Grid::addObj err, obj is already part of Layout\n";
        return false;
    }
    obj->posLocked = true;
    linkedObjects[loc.x][loc.y] = obj;
    obj->addComponent(new LayoutHelperComponent(this, size.x * loc.x + loc.y));
    log(LOG_INFO) << "Object " << obj->getName() << " added to Grid " << this << "\n";
    return true;
}

bool Grid::removeObj(int id, bool manual = true)
{
    std::div_t result = std::div(id, size.x);
    if (result.rem > size.y)
    {
        log(LOG_WARN) << "Grid::removeObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    Object *obj = linkedObjects[result.rem][result.quot];
    if (manual)
    {
        obj->removeComponent(obj->getComponent<LayoutHelperComponent>());
    }
    log(LOG_INFO) << "Object of id: " << id << " removed from grid " << this << "\n";
    linkedObjects[result.rem][result.quot] = nullptr;
    return true;
}

void Grid::update()
{
    Object::update();
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            if (linkedObjects[j][i] != nullptr)
            {
                linkedObjects[j][i]->move(calculateSpaceCoordinates({j, i}), true);
            }
        }
    }
    // log(LOG_INFO) << linkedObjects[0][0] << "\n";
}

Vect Grid::calculateSpaceCoordinates(iVect loc)
{
    Vect result = {loc.x * cellSize + cellSize / 2, loc.y * cellSize + cellSize / 2};
    return pos + result - gridCenter;
}

Family::Family(Scene *scene) : Layout(scene)
{
    familySize = 0;
}

bool Family::addObj(Object *obj, Vect objPos)
{

    if (obj == nullptr)
    {
        return false;
    }
    if (obj->getComponent<LayoutHelperComponent>() != nullptr)
    {
        log(LOG_WARN) << "Family::addObj err, obj is already part of Layout\n";
        return false;
    }
    obj->posLocked = true;
    linkedObjects.push_back(obj);
    lookupOffset.push_back(objPos);
    LayoutHelperComponent *helper = new LayoutHelperComponent(this, familySize);
    obj->addComponent(helper);
    obj->move(objPos, true);
    familySize++;
    log(LOG_INFO) << "Object " << obj->getName() << " added to family " << this << "\n";
    return true;
}

bool Family::removeObj(int id, bool manual = true)
{
    if (id >= 0 && id < familySize)
    {
        Object *obj = linkedObjects[id];
        obj->removeComponent(obj->getComponent<LayoutHelperComponent>());
        linkedObjects.erase(linkedObjects.begin() + id);
        lookupOffset.erase(lookupOffset.begin() + id);
        familySize--;
        return true;
    }
    return false;
}

void Family::update()
{
    Object::update();
    for (int i = 0; i < familySize; i++)
    {
        linkedObjects[i]->move(pos + lookupOffset[i], true);
    }
}