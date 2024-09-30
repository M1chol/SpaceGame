# All primitive systems

I implemented two vector like classes
#### `Vect` Structure
- **Purpose**: Represents a 2D vector with `double` precision, used for handling positions, directions, and operations in floating-point space.
- **Attributes**:
  - `x` and `y`: Two `double` values representing the vector's coordinates.
- **Key Methods**:
  - **Operator Overloads**: `+`, `-`, `*`, `+=`, and `*=` for vector arithmetic and scalar multiplication.
  - **magnitude()**: Calculates the length of the vector.
  - **normalized()**: Returns a unit vector (direction vector with length 1).
  - **toIVect()**: Converts this `Vect` to an `iVect`, effectively casting its values to integers.

#### `iVect` Structure
- **Purpose**: Represents a 2D vector with `int` precision, typically used when integer-based grid positioning is required - like rendering at pixel position.
- **Attributes**:
  - `x` and `y`: Two `int` values representing the vector's coordinates.
- **Key Methods**:
  - **Operator Overloads**: `+`, `*`, `+=`, and `*=` for vector arithmetic and scalar multiplication in integer space.

These structures provide core functionality for vector mathematics, handling both floating-point and integer-based operations within the game world.

---

## Game world structures

There are 3 primitive structures their respective definitions are located in `gPrimitives.h`
- `Sceme`
- `Object`
- `Component`

### `Scene` Class
- **Purpose**: Manages a collection of `Object` instances and handles the overall update and rendering logic for a specific group of `Object` instances in the game.
- **Attributes**:
  - `sceneRenderer`: The SDL renderer associated with the scene.
  - `objectList`: A list of all objects present in the scene.
  - `name`: The name of the scene.
  - `toBeRemoved`: A list of objects scheduled for destruction.
- **Key Methods**:
  - **addObject() / removeObject()**: Adds or removes objects to/from the scene.
  - **Update()**: Updates all active objects in the scene.
  - **handleCollisions()**: Manages collision detection and resolution between objects.
  - **getObjectByName()**: Retrieves an object by its name.
  - **getObjectByTag()**: Retrieves objects by a specific tag.
  - **removeScheduled()**: Removes objects that have been marked for destruction.
  - **setName() / getName()**: Sets or gets the scene's name.
  - **getRenderer()**: Returns the linked SDL renderer.


### `Object` Class
- **Purpose**: Represents an entity within the game world, which have a position, array of components and functions such as rendering and updating.
- **Attributes**:
  - `pos`: Stores the position of the object using a `Vect`.
  - `isActive`: Determines whether the object is active or not.
  - `posLocked`: When `true`, prevents the object from being moved.
  - `linkedTags`: A list of tags associated with the object for identification and querying.
  - `componentList`: Holds a list of `Component` objects that define additional behaviors or attributes for the object.
  - `linkedScene`: Points to the `Scene` in which this object exists.
  - `name`: The name of the object.
- **Key Methods**:
  - **render()**: Renders the object by invoking the render function of each linked component.
  - **update()**: Updates the object by invoking the update function of each linked component.
  - **lateUpdate()**: Executes logic that should happen after all objects have updated (e.g., solving collisions).
  - **move()**: Moves the object to a new position, optionally forcing the move even if `posLocked` is `true`.
  - **destroy()**: Marks the object for destruction.
  - **addComponent() / removeComponent()**: Manages components by adding or removing them.
  - **getComponent()**: Retrieves a specific component by type.
  - **setScene() / getScene()**: Associates or retrieves the linked scene.
  - **setName() / getName()**: Sets or gets the object's name.

```cpp
Object player;
SpriteComponent *component = new SpriteComponent(gRenderer)
player.addComponent(component);
```
This block of code is creating new Object and then linking newly created `SpriteComponent` to it.

### `Component` Class
- **Purpose**: Provides modular functionality that can be added to an `Object`, such as behaviors, physics, rendering, etc.  
- **Attributes**:  
  - `parent`: Points to the `Object` this component is attached to.  
**Key Methods**:  
  - **whenLinked()**: Called when the component is attached to an object, allowing for initialization.
  - **render()**: Optionally renders the component (if applicable).
  - **update()**: Optionally updates the component (if applicable).
  - **setParent() / getParent()**: Associates or retrieves the parent object.

`Component` is an abstract parent class for:   
- `SpriteComponent`
- `RigidBodyComponent`
- `SpawnerComponent`
- `TextComponent`
- `LayoutHelperComponent`  
Later documented in [components.md](https://github.com/M1chol/SpaceGame/blob/master/doc/components.md)