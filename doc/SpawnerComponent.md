# Spawner Component
spawner component is component used to spawn Objects of `genericBullet` class or derived classes. It is using pooling for memory menagment. It stores a list of current bullets of type `bulletType` (eg. genericBullet). New bullets are pushed to the back of the list. On every frame we check each bullet in list if its LifeSpan `aliveFor` reached `bulletLife`, and if so we deactivate it. This enables very easy way of checking if we need to create new object. We check first object and if it is active we know that all bullets are beeing used. If we are reusing the object we push it to the end of the list.

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