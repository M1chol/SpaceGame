#include "engine.h"

Layout::Layout(Scene *scene) : Object(scene) {}
Layout::~Layout() {};
bool Layout::addObj() {};
bool Layout::removeObj(int id) {};

Grid::Grid(Scene *scene, iVect setSize, double setCellSize) : Layout(scene)
{
    size = setSize;
    cellSize = setCellSize;
    renderer = scene->getRenderer();
    gridCenter = {size.x * cellSize / 2, size.y * (double)cellSize / 2};
    linkedObjects.resize(size.y);
    for (auto &row : linkedObjects)
    {
        row.resize(size.x);
    }
}

void Grid::render()
{
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
    LOG_INIT_CERR();
    if (loc.x > size.x - 1 || loc.y > size.y - 1)
    {
        log(LOG_WARN) << "Grid::addObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    if (linkedObjects[loc.x][loc.y] != nullptr)
    {
        log(LOG_WARN) << "Grid::adObj err, space is already occupied\n";
        return false;
    }
    if (obj->getComponent<LayoutHelperComponent>() != nullptr)
    {
        log(LOG_WARN) << "Grid::adObj err, obj is already part of Layout\n";
        return false;
    }
    obj->posLocked = true;
    linkedObjects[loc.x][loc.y] = obj;
    obj->addComponent(new LayoutHelperComponent(this, size.x * loc.x + loc.y));
    return true;
}

bool Grid::removeObj(int id)
{
    LOG_INIT_CERR();
    std::div_t result = std::div(id, size.x);
    if (result.rem > size.y)
    {
        log(LOG_WARN) << "Grid::removeObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    Object *obj = linkedObjects[result.rem][result.quot];
    obj->removeComponent(obj->getComponent<LayoutHelperComponent>());
    return true;
}

void Grid::update()
{
    Object::update();
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            if (linkedObjects[i][j] != nullptr)
            {
                linkedObjects[i][j]->move(calculateSpaceCoordinates({i, j}), true);
            }
        }
    }
}

Vect Grid::calculateSpaceCoordinates(iVect loc)
{
    Vect result = {loc.x * cellSize + cellSize / 2, loc.y * cellSize + cellSize / 2};
    return pos + result - gridCenter;
}