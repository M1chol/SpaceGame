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
    bool render(iVect offset, float scale = 0.0);
    /*
    Renders the texture assigned to object to screen centered at parent object
    @param scale set the size of image, 1 = 100%
    */
    bool render(float scale = 0.0);
    /*
    Renders the texture assigned to object to screen centered at parent object
    */
    bool render();
    /* This function runs every time Object is linked to new Scene*/
    void whenLinked() override;
    /* Returns dimentions of texture*/
    iVect *getDim();
    void setScale(float newScale);

private:
    SDL_Texture *texture;
    std::string path;
    SDL_Renderer *gRenderer;
    SDL_Rect *renderBox;
    iVect *dim;
    float scale;
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
    /* Checks if `Object` is colliding with other `Object`. Value can be checked using `Object` pointer OR `TAG` @param objComp pointer to objects RB @param tag search by tag @returns pointer to found object, nullptr if collision is not accouring*/
    RigidBodyComponent *isColliding(RigidBodyComponent *objComp);
    /* Checks if `Object` is colliding with other `Object`. Value can be checked using `Object` pointer OR `TAG` @param objComp pointer to objects RB @param tag search by tag @returns pointer to found object, nullptr if collision is not accouring*/
    RigidBodyComponent *isColliding(TAG tag, bool debug = false);
    /* add RB to colliding list @param objComp RB of object to be added to list */
    void solveCollision(RigidBodyComponent *objComp);
    // TODO: TEMP
    std::vector<RigidBodyComponent *> collisionList;

private:
    std::vector<iVect> hitBox;

    double mass;
    double maxSpeed;
    double energyLoss;
    // double elasticity;
    Vect velocity;
    Vect force;
    // Vect momentum;
    bool isTrigger;
};

template <typename bulletType>
class SpawnerComponent : public Component
{
public:
    // Create SpawnerComponent @param setShootOffset sets shoot offset @param setCooldown sets shoot cooldown @param setBulletLifeSpan sets Bullet time to deconstruction
    SpawnerComponent(Vect setShootOffset, double setCooldown, double setBulletLifeSpan);
    // Handles shooting
    bool shoot();
    // Set new cooldown of Spawner @param newCooldown new cooldown
    void setCooldown(double newCooldown);

protected:
    bool update() override;
    void whenLinked() override;

private:
    Vect pos;
    double cooldown;
    double cooldownTimer;
    int poolsize;
    double bulletLifeSpan;
    Vect shootOffset;
    std::vector<std::shared_ptr<bulletType>> pool;
};

#endif