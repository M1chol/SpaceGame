#ifndef Components
#define Components

#include "engine.h"

using namespace std;

#pragma once

class SpriteComponent : public Component{
    public:

        // Constructor
        SpriteComponent(SDL_Renderer* gRenderer, string path);

        // Deconstructor
        ~SpriteComponent();

        // Loads texture on specified path returns false if it fails
        bool load(string path);

        void free();

        void simpleRender(iVect pos);

        void render(iVect* coordinates, iVect* offset, SDL_Rect* clip = NULL);

        iVect* getDim();

    private:
        SDL_Texture* texture;
        SDL_Renderer* gRenderer;
        iVect* dim;
};

#endif