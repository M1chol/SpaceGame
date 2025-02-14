### SpriteComponent Class

The `SpriteComponent` class is used to add sprites to any `Object`. It allows you to load textures, render them, and manage their properties such as scale and dimensions.

#### Simple Use Case

Here is an example of how to use the `SpriteComponent`:

```cpp
Object player;
SpriteComponent *playerSprite = new SpriteComponent("res/player-placeholder.png", &player);
```

After creating an `Object` and linking a `SpriteComponent` to it, you can load a texture into memory using the `load` function. Alternatively, you can specify the texture path when creating the `SpriteComponent`.

```cpp
playerSprite->load("res/player-placeholder.png");
```

To render the sprite, you can call the `render` function directly from the pointer:

```cpp
playerSprite->render();
```

Or you can call `render` on the parent `Object`, which will render all its components:

```cpp
player.render();
```

#### Full Example

```cpp
Object player;
player.addComponent(new SpriteComponent("res/player-placeholder.png", &player));
// Render directly from the SpriteComponent
// static_cast<SpriteComponent*>(player.getComponent(0))->render();
player.render(); // Render all components linked to the Object
```

#### Key Functions

- **`SpriteComponent(std::string path = "", Object *parent = nullptr)`**  
  Constructor to create a `SpriteComponent`. Optionally, you can specify a texture path and the parent `Object`.

- **`bool load(std::string path = "")`**  
  Loads a texture from the specified path. If no path is provided, it uses the path stored in the instance. Returns `true` if successful.

- **`bool render(iVect offset, float scale = 0.0)`**  
  Renders the texture at the position of the parent `Object` with an optional offset and scale.

- **`bool render(float scale = 0.0)`**  
  Renders the texture centered at the parent `Object` with an optional scale.

- **`bool render()`**  
  Renders the texture centered at the parent `Object`.

- **`iVect *getDim()`**  
  Returns the dimensions of the texture.

- **`void setScale(float newScale)`**  
  Sets the scale of the texture.

---

### RigidBodyComponent Class

The `RigidBodyComponent` class is used to add physics and collision detection to an `Object`. It allows you to apply forces, set mass, define hitboxes, and handle collisions.

#### Key Functions

- **`void applyForce(Vect newForce)`**  
  Applies a force to the parent `Object`.

- **`void setMass(double newMass)`**  
  Sets the mass of the parent `Object`.

- **`void setEnergyLoss(double newEnergyLoss)`**  
  Sets the energy loss (value between 0 and 1) for the parent `Object`.

- **`void setCollision(SDL_Rect *newHitBox, bool isTrigger = false)`**  
  Sets the collision hitbox for the parent `Object`. The `isTrigger` parameter determines if the hitbox is a trigger.

- **`SDL_Rect *getHitBox()`**  
  Returns the hitbox of the `RigidBodyComponent`.

- **`RigidBodyComponent *isColliding(RigidBodyComponent *objComp)`**  
  Checks if the parent `Object` is colliding with another `Object` using its `RigidBodyComponent`.

- **`RigidBodyComponent *isColliding(TAG tag)`**  
  Checks if the parent `Object` is colliding with another `Object` using a `TAG`.

#### Use Case

Here is an example of how to use the `RigidBodyComponent` to create a moving player with collision detection:

```cpp
PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    setName("PlayerObject");
    PlayerRB = new RigidBodyComponent(10, this);
    addComponent(PlayerRB);
    forceToApply = {0.0, 0.0};
    playerSpeed = 350.0;
    pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    PlayerRB->setEnergyLoss(0.03);
    SDL_Rect box = {66, 70, 132, 140}; // Example hitbox
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
        forceToApply += {1.0, 0.0};
    }
    PlayerRB->applyForce(forceToApply.normalized() * playerSpeed);
    forceToApply = {0.0, 0.0};
}
```

---

### SpawnerComponent Class

The `SpawnerComponent` is used to spawn `Objects` of a specified type (e.g., `genericBullet`). It uses object pooling for efficient memory management.

#### Key Functions

- **`bool shoot()`**  
  Spawns a new object (e.g., a bullet) if the cooldown has elapsed.

- **`void setCooldown(double newCooldown)`**  
  Sets the cooldown time between spawns.

#### Use Case

Here is an example of how to use the `SpawnerComponent` to create a player that shoots bullets:

```cpp
// player.h
SpawnerComponent<genericBullet> *bulletSpawner;
```

```cpp
// player.cpp
PlayerObject::PlayerObject(Scene *scene) : Object(scene)
{
    setName("PlayerObject");
    bulletSpawner = new SpawnerComponent<genericBullet>({0, 0}, 0.2, 2);
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

---

### TextComponent Class

The `TextComponent` is a specialized `SpriteComponent` used to render text on the screen. It supports custom fonts, colors, and positions.

#### Key Functions

- **`bool load(std::string newMessage = "", SDL_Color color = {255, 255, 255}, std::string fontPath = "")`**  
  Loads a new text message with the specified color and font.

- **`void setFont(std::string fontPath, int fontSize = 20)`**  
  Sets the font and font size for the text.

#### Use Case

```cpp
Object textObject;
TextComponent *text = new TextComponent("Hello, World!", {100, 100}, "res/font.ttf", &textObject);
textObject.addComponent(text);
text->load("Updated Text", {255, 0, 0});
textObject.render();
```

---

### LayoutHelperComponent Class

The `LayoutHelperComponent` is used to assist with managing layouts. It links an `Object` to a `Layout` and provides an ID for identification.

#### Key Functions

- **`LayoutHelperComponent(Layout *setLayout = nullptr, int setId = -1)`**  
  Constructor to create a `LayoutHelperComponent` with an optional layout and ID.
