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
        
        // TODO: change function type to bool and handle errors
        void render(iVect offset, float scale);
        void render(float scale);
        void render();

        iVect* getDim();

    private:
        SDL_Texture* texture;
        SDL_Renderer* gRenderer;
        SDL_Rect* renderBox;
        iVect* dim;
};

#endif