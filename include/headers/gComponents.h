#ifndef Components
#define Components

#include "engine.h"

#pragma once

class SpriteComponent : public Component
{
public:
    /* Create SpriteComponent @param path optional path to image that will be loaded automaticly*/
    SpriteComponent(std::string path = "");

    /* Destroy SpriteComponent*/
    ~SpriteComponent() override;

    /* Loads texture on specified path @returns true if successfull */
    bool load(std::string path);

    // void destroy() override;

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
    /* This function runs every time Object is linked to new Scene*/
    void whenLinked() override;
    /* Returns dimentions of texture*/
    iVect *getDim();

private:
    SDL_Texture *texture;
    std::string path;
    SDL_Renderer *gRenderer;
    SDL_Rect *renderBox;
    iVect *dim;
};

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent(double mass = 0);
    ~RigidBodyComponent() override;
    bool update() override;
    bool render() override;
    void whenLinked() override;
    void applyForce(Vect newForce);
    void setMass(double newMass);
    void setEnergyLoss(double newEnergyLoss);
    void setCollision(std::vector<iVect> *newHitBox);
    std::vector<iVect> &getHitBox();
    bool hasCollision;
    bool isColliding(RigidBodyComponent *objComp);
    void setColliding(RigidBodyComponent *objComp);

private:
    std::vector<iVect> hitBox;
    std::vector<Object *> collisionList;
    double mass;
    double maxSpeed;
    double energyLoss;
    Vect velocity;
    Vect force;
};

#endif