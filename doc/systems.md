# All classes and functions

There are 3 primitive structures their respective definitions are located in `gPrimitives.h`
- `Sceme`
- `Object`
- `Component`

## Scene class
Scene class holds list of `Object` It is used to keep all Objects for ease of clearing memory. It holds name for debugging purpouses.


## Object class
Object class is the most primitive object that can be created. It has position and a list of linked components.

Example usecase:
```cpp
Object player;
component = new SpriteComponent(gRenderer)
player.addComponent(component);
```
This block of code is creating new Object and then linking newly created `SpriteComponent` to it.

## Component class
`Component` is an abstract parent class for:   
- `SpriteComponent`

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


Other functions