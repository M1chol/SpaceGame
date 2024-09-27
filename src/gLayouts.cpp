#include "engine.h"

Grid::Grid(Scene *scene, iVect setSize, double setCellSize) : Object(scene)
{
    size = setSize;
    cellSize = setCellSize;
}

Grid::~Grid()
{
    
}