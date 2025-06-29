#ifndef Components
#define Components

#include "engine.h"

#pragma once

class SpriteComponent : public Component
{
public:
    /* Create SpriteComponent @param path optional path to image that will be loaded automaticly*/
    SpriteComponent(std::string path = "", Object *parent = nullptr);

    /* Destroy SpriteComponent*/
    ~SpriteComponent() override;

    /* Loads texture on specified path @param path in not set loads path saved in instance @returns true if successfull */
    virtual bool load(std::string path = "");

    // void destroy() override;

    /*
    Renders the texture assigned to object to screen at the position of linked parent
    @param offset set the offset from parent position
    @param scale set the size of image, 1 = 100%
    @return true if success else false
    */
    bool render(int index, iVect offset, float scale);
    /*
    Renders the texture assigned to object to screen centered at parent object
    @param scale set the size of image, 1 = 100%
    */
    bool render(int index, float scale);
    bool render() override;
    /*
    /* This function runs every time Object is linked to new Scene*/
    void whenLinked() override;
    /* Returns dimentions of texture*/
    iVect getDim();
    void setScale(float newScale);
    void setSheetIndex(int index);
    void setOffset(iVect newOffset) { offset = newOffset; };
    void setCentered() { offset = center; };
    void rotate(double newRotation) { rotation = newRotation; }

    iVect getCenter() { return center; };

    iVect offset;

protected:
    SDL_Renderer *gRenderer;
    SDL_Texture *texture;
    iVect dim;
    iVect center;
    std::string path;
    float scale;
    void calculateCenter();

private:
    SDL_Rect *renderBox;
    int sheetSize;
    int sheetIndex;
    double rotation;
};

class RigidBodyComponent : public Component
{
public:
    /* Creates RigidBodyComponent @param mass mass of the object, default 0*/
    RigidBodyComponent(double mass = 0, Object *parent = nullptr);
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
    void setCollision(SDL_Rect *newHitBox, bool isTrigger = false);
    /* Returns hitbox of RigidBody */
    SDL_Rect *getHitBox();
    /* Return calculated iVect from SDL_Rect @param index 0 for top-left 1 for bottom-right*/
    iVect getHitBox(int index);
    // TODO: move hasCollision to private, why is it here?
    /* If true collisions will be checked for this object*/
    bool hasCollision;
    /* Checks if `Object` is colliding with other `Object`. Value can be checked using `Object` pointer OR `TAG` @param objComp pointer to objects RB @param tag search by tag @returns pointer to found object, nullptr if collision is not accouring*/
    RigidBodyComponent *isColliding(RigidBodyComponent *objComp);
    /* Checks if `Object` is colliding with other `Object`. Value can be checked using `Object` pointer OR `TAG` @param objComp pointer to objects RB @param tag search by tag @returns pointer to found object, nullptr if collision is not accouring*/
    RigidBodyComponent *isColliding(TAG tag);
    /* add RB to colliding list @param objComp RB of object to be added to list */
    void solveCollision(RigidBodyComponent *objComp);

private:
    SDL_Rect hitBox;
    std::vector<RigidBodyComponent *> collisionList;
    double mass;
    double energyLoss;
    // double elasticity;
    Vect velocity;
    Vect force;
    // Vect momentum;
    bool isTrigger;
    bool isClickable;
    SDL_Renderer *renderer;
};

class TextComponent : public SpriteComponent
{
public:
    TextComponent(std::string setMessage = "", iVect newOffset = {0, 0}, std::string fontPath = "", int fontSize = 20, Object *parent = nullptr);
    void setColor(SDL_Color newColor);
    // Shadows the setScale function from SpriteComponent calls setScale
    void setScale(float newScale) { setSize((int)newScale); };
    void setSize(int newSize);
    void setFont(std::string fontPath, int fontSize = 20);

private:
    bool load(std::string newMessage = "", SDL_Color color = {255, 255, 255, 255}, std::string fontPath = "", int fontSize = 20);
    void whenLinked() override;
    bool update() override;
    TTF_Font *font;
    std::string fontPath;
    SDL_Color color;
    int fontSize;
};

class Layout;

#endif