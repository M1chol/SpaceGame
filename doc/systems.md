# All classes and functions

- `main/bool init()` funcition that initializes SDL and creates game window returns false if could not initialize

- `main/void close()` function that frees resources, destroys game window and closes SDL

There are 2 primitive structures
- `Object`
- `Component`
## Object class
Object class is the most primitive object that can be created. It has position and a list of linked components.

```cpp
Object();
~Object();
Vect pos; // position of object
void destroy(); // frees resources
void addComponent(Component* comp); // link new component
Component* getComponent(int componentId); // get pointer to component at index
int render(); // calls render on all components
```
Example usecase:
```cpp
Object player;
component = new SpriteComponent(gRenderer)
player.addComponent(component);
```
This block of code is creating new Object and then linking newly created `SpriteComponent` to it.

## Component class

```cpp
void setParent(Object* parent); // set parent object
virtual bool render(); // if child object can be renderd ralls render else returns 0
```

`Component` is an abstract parent class for:
- `SpriteComponent`

### SpriteComponent class
Yo can use this class to add sprites to any Object.
```cpp
SpriteComponent(SDL_Renderer* gRenderer, string path);
~SpriteComponent();
bool load(string path); // Loads texture on specified path
void free();

// Renders the texture assigned to object to screen at the position of linked parent
bool render(iVect offset, float scale);
bool render(float scale);
bool render() override;
iVect* getDim(); // returns original dimentions of loaded texture
```
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
