#include "engine.h"

Layout::Layout(Scene *scene) : Object(scene)
{
    ID = LayoutGetID();
}

void Layout::destroy()
{
    for (Object *obj : linkedObjects)
    {
        obj->destroy();
    }
    Object::destroy();
}

bool Layout::addObj() {return true;};
bool Layout::removeObj(int id, bool manual = true) {return true;};

Grid::Grid(Scene *scene, iVect setSize, double setCellSize) : Layout(scene)
{
    ID = LayoutGetID();
    size = setSize;
    std::string newName = "Grid " + std::to_string(ID);
    setName(newName);
    cellSize = setCellSize;
    calculateGridCenter();
    renderer = scene->getRenderer();
    log(LOG_INFO) << "Created " << getName() << "\n";
}

Vect Grid::calculateGridCenter()
{
    gridCenter = {size.x * cellSize / 2, size.y * (double)cellSize / 2};
    return gridCenter;
}

void Grid::render()
{
    if (drawDebug)
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
}

int Grid::iVectToId(iVect loc)
{
    return size.x * loc.y + loc.x;
}
iVect Grid::idToIVect(int id)
{
    std::div_t result = std::div(id, size.x);
    return {result.rem, result.quot};
}

bool Grid::addObj(iVect loc, Object *obj)
{

    if (loc.x > size.x - 1 || loc.y > size.y - 1)
    {
        log(LOG_WARN) << "Grid::addObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    for (int objId : linkedObjectsId)
    {
        if (objId == iVectToId(loc))
        {
            log(LOG_WARN) << "Grid::addObj error, space already occupied\n";
            return false;
        }
    }
    if (obj->getComponent<LayoutHelperComponent>() != nullptr)
    {
        log(LOG_WARN) << "Grid::addObj err, obj is already part of another Layout\n";
        return false;
    }
    obj->posLocked = true;
    linkedObjects.push_back(obj);
    linkedObjectsId.push_back(iVectToId(loc));
    obj->addComponent(new LayoutHelperComponent(this, iVectToId(loc)));
    obj->move(calculateSpaceCoordinates(loc), true);
    log(LOG_INFO) << "Object " << obj->getName() << " added to Grid " << this << "\n";
    return true;
}

bool Grid::addObj(int id, Object *obj)
{
    return addObj(idToIVect(id), obj);
}

bool Grid::removeObj(int id, bool manual = true)
{
    iVect loc = idToIVect(id);
    if (loc.x > size.y || id < 0)
    {
        log(LOG_WARN) << "Grid::removeObj error index out of bounds for grid " << this << "\n";
        return false;
    }
    for (int i = 0; i < linkedObjects.size(); i++)
    {
        if (id == linkedObjectsId[i])
        {
            Object *obj = linkedObjects[i];
            if (manual)
            {
                obj->removeComponent(obj->getComponent<LayoutHelperComponent>());
            }
            linkedObjects.erase(linkedObjects.begin() + i);
            linkedObjectsId.erase(linkedObjectsId.begin() + i);
            log(LOG_INFO) << "Object of id: " << id << " removed from grid " << this << "\n";
            return true;
        }
    }
    log(LOG_INFO) << "Grid::removeObj (" << this << ") failed, item not found\n";
    return false;
}

void Grid::update()
{
    Object::update();
    for (int i = 0; i < linkedObjects.size(); i++)
    {
        linkedObjects[i]->move(calculateSpaceCoordinates(idToIVect(linkedObjectsId[i])), true);
    }
}

Vect Grid::calculateSpaceCoordinates(iVect loc)
{
    Vect result = {loc.x * cellSize + cellSize / 2, loc.y * cellSize + cellSize / 2};
    return pos + result - gridCenter;
}

Family::Family(Scene *scene) : Layout(scene)
{
    ID = LayoutGetID();
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