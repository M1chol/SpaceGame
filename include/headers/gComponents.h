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
    /* Creates RigidBodyComponent @param mass mass of the object, default 0*/
    RigidBodyComponent(double mass = 0);
    ~RigidBodyComponent() override;
    /* Updates velocity and position of paren `Object`*/
    bool update() override;
    /* If global variable `drawHitbox` is true renders hitboxes*/
    bool render() override;
    void whenLinked() override;
    /* Apply new force on parent `Object` @param newForce new force*/
    void applyForce(Vect newForce);
    /* Set or change the mass of object @param newMass new mass*/
    void setMass(double newMass);
    /* Set the energy loss of parent Object. @param newEnergyLoss value from 0 to 1*/
    void setEnergyLoss(double newEnergyLoss);
    /* Change or set new collision detection box @param newHitBox array of `iVect`'ors*/
    void setCollision(std::vector<iVect> *newHitBox, bool isTrigger = false);
    /* Returns hitbox of RigidBody */
    std::vector<iVect> &getHitBox();
    /* If true collisions will be checked for this object*/
    bool hasCollision;
    /* Returns true if Object is colliding with `objComp` @param objComp pointer to objects RB*/
    bool isColliding(RigidBodyComponent *objComp);
    /* add RB to colliding list @param objComp RB of object to be added to list */
    void solveCollision(RigidBodyComponent *objComp);

private:
    std::vector<iVect> hitBox;
    std::vector<RigidBodyComponent *> collisionList;
    double mass;
    double maxSpeed;
    double energyLoss;
    Vect velocity;
    Vect force;
    bool isTrigger;
};

#endif