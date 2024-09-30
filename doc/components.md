### SpriteComponent class
You can use this class to add sprites to any `Object`.   
   
Simple usecase:   
Here is continuation for example explored in Object section.   
```cpp
Object player;
playerSprite* = new SpriteComponent(gRenderer);
player.addComponent(component);
```
After creating Object and linking `SpriteComponent` to it you can load texture to memory using `load` function. You can also do it while creating new `SpriteComponent` by specifying `path` argument.

```cpp
playerSprite->load("res/player-placeholder.png");
```
And then render it to screen calling directly from pointer
```cpp
playerSprite->render();
```
or by calling `render` on parent `Object`
```cpp
player.render();
```
full example:
```cpp
Object player;
player.addComponent(new SpriteComponent(gRenderer, "res/player-placeholder.png"));
//static_cast<SpriteComponent*>(player.getComponent(0))->render(); // Calling render by directly referencing SpriteComponent linked to Object
player.render(); // Calling render on all Components linked to Object
```
### RigidBodyComponent class

Rigidbody component is used when you want to make `Object` move or set its colisions. There are several functions aviable.

- void applyForce(Vect newForce)
- void setMass(double newMass)
- void setEnergyLoss(double newEnergyLoss)
- void setCollision(std::vector<iVect> *newHitBox, bool isTrigger = false)
- std::vector<iVect> &getHitBox()
- bool hasCollision
- bool isColliding(RigidBodyComponent *objComp)

#### Usecase
This example shows how to use Rigidbody component to make moving player. In addition hitbox is set using `setCollision()` function.
```cpp

PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    setName("PlayerObject");
    PlayerRB = new RigidBodyComponent(10);
    addComponent(PlayerRB);
    forceToApply = {0.0, 0.0};
    playerSpeed = 350.0;
    pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    PlayerRB->setEnergyLoss(0.03);
    std::vector<iVect> box = {{66, 70}, {-66, -70}};
    PlayerRB->setCollision(&box);
}

void PlayerObject::update()
{
    Object::update();
    if (isKeyDown(SDL_SCANCODE_W))
    {
        forceToApply += {0.0, -1.0};
    }
    if (isKeyDown(SDL_SCANCODE_S))
    {
        forceToApply += {0.0, 1.0};
    }
    if (isKeyDown(SDL_SCANCODE_A))
    {
        forceToApply += {-1.0, 0.0};
    }
    if (isKeyDown(SDL_SCANCODE_D))
    {
        forceToApply += {1.0, 0};
    }
    PlayerRB->applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}

```

## SpawnerComponent class
spawner component is component used to spawn `Objects` of `genericBullet` class or other derived classes. It is using pooling for memory menagment. It stores a list of current bullets of type `bulletType` (eg. genericBullet). New bullets are pushed to the back of the list. On every frame we check each bullet in list if its LifeSpan `aliveFor` reached `bulletLife`, and if so we deactivate it. This enables very easy way of checking if we need to create new object. We check first object and if it is active we know that all bullets are beeing used. If we are reusing the object we push it to the end of the list.   
   
functions aviable:
- bool shoot();
- void setCooldown(double newCooldown);


## Use case
```cpp
// player.h
SpawnerComponent<genericBullet> *bulletSpawner;
// Rest of the code 
```

```cpp
// player.cpp
PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    setName("PlayerObject");
    bulletSpawner = new SpawnerComponent<genericBullet>(center, 0.2, 2);
    addComponent(bulletSpawner);
}

void PlayerObject::update()
{
    Object::update();
    if (isKeyDown(SDL_SCANCODE_SPACE))
    {
        bulletSpawner->shoot();
    }
}
```