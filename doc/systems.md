## All classes and functions

- `main/bool init()` funcition that initializes SDL and creates game window returns false if could not initialize

- `main/void close()` function that frees resources, destroys game window and closes SDL

There are 2 primitive structures
- `gObject`
- `Component`

`Component` is a parent class for:
- `SpriteComponent`

`Component` can be linked to `gObject` using `gObject.addComponent()` for example

```cpp
gObject player;
component = new SpriteComponent(gRenderer, "player.png")
player.addComponent(component);
```
The idea behind this is to create scalable system.

### SpriteComponent (Sprite)
texture loading supports png, jpg
`Sprite(SDL_Renderer* renderer, string path)` - creates texture object bound to provided renderer. path - optional, if provided automaticly runs Sprite.load.
`bool load(string path)` - used to load texture on specified path
`void free()` - called to destroy object
`void render(iVect* coordinates, iVect* offset, SDL_Rect* clip = NULL);` - renders texture to renderer
`iVect* getDim();` - returns dimentions of image