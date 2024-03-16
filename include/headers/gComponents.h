
#ifndef Components
#define Components

#include <string>
#include "logger.h"
#include <SDL.h>
#include "gMath.h"
using namespace std;

#pragma once

class LTexture{
    public:

        // Constructor
        LTexture();

        // Deconstructor
        ~LTexture();

        bool loadFromFile(string path);

        void free();

        void render(iVect* coordinates, iVect* offset, SDL_Rect* clip = NULL);

        iVect* getDim();

    private:
        SDL_Texture* texture;
        iVect* dim;
};

#endif