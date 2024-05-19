#ifndef Components
#define Components

#include "engine.h"

#pragma once

class SpriteComponent : public Component
{
public:
    // Constructor
    SpriteComponent(std::string path = "");

    // Deconstructor
    ~SpriteComponent();

    // Loads texture on specified path returns false if it fails
    bool load(std::string path);

    void free();

    /*
    Renders the texture assigned to object to screen at the position of linked parent
    @param offset set the offset from parent position
    @param scale set the size of image, 1 = 100%
    @return true if success else false
    */
    bool render(iVect offset, float scale);
    /*
    Renders the texture assigned to object to screen centered at parent object
    @param scale set the size of image, 1 = 100%
    */
    bool render(float scale);
    /*
    Renders the texture assigned to object to screen centered at parent object
    */
    bool render() override;
    void whenLinked() override;

    iVect *getDim();

private:
    SDL_Texture *texture;
    std::string path;
    SDL_Renderer *gRenderer;
    SDL_Rect *renderBox;
    iVect *dim;
};

#endif