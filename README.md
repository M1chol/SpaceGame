## development in early stages

license files available in /legal   
documentation avaiable in /doc

Source-avaiable Space Invaders style c++ game based on SDL

Target os:
  1. Windows
  2. Linux

## Installing
For windows simply __Use provided binary__ you can find latest exe file in releases   
   
__Build from source__   
Windows dependencies  
- [git](https://git-scm.com/download/win)
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  (optional)
    
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
If you didn't install `mingw32-make` instead launch   
```
g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o Main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
```

Linux dependencies      
1. git
2. make
3. SDL2
4. SDL2_image
   
Start by installing SDL for your system. You can use package menager or [build from surce](https://github.com/libsdl-org/SDL). You will also need SDL2_image

__Arch__ `sudo pacman -S sdl2 sdl2_image`   
__Ubuntu__ `sudo apt-get install libsdl2 libsdl2-image` (NOT TESTED)   
Then   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
make
```
