## Functions and how they work

- `main/bool init()` funcition that initializes SDL and creates game window returns false if could not initialize

- `main/void close()` function that frees resources, destroys game window and closes SDL

- `main/SDL_Surface* loadSurface(string path)` loads specefied bmp image to surface checks if loaded successfuly and returns pointer to that surface

- `main/bool loadMedia()` loads static BMP to static surface, returns false if media failed to load