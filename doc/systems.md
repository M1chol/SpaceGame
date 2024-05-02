# All classes and functions

- `main/bool init()` funcition that initializes SDL and creates game window returns false if could not initialize

- `main/void close()` function that frees resources, destroys game window and closes SDL

There are 2 primitive structures
- `gObject`
- `Component`
## gObject class
gObject class is the most primitive object that can be created. It has position and a list of linked components.

```cpp
gObject();
~gObject();
Vect pos; // position of object
void destroy(); // frees resources
void addComponent(Component* comp); // link new component
Component* getComponent(int componentId); // get pointer to component at index
int render(); // calls render on all components
```
Example usecase:
```cpp
gObject player;
component = new SpriteComponent(gRenderer, "player.png")
player.addComponent(component);
```

## Component class

```cpp
void setParent(gObject* parent); // set parent object
virtual bool render(); // if child object can be renderd ralls render else returns 0
```

`Component` is an abstract parent class for:
- `SpriteComponent`

### SpriteComponent class

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