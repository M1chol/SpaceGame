## Space Game SDL Project
This is my source-avaiable c++ game made with SDL library. As of now the game does not hava a proper name. Feel free to fork / modify / mod the game.

all license files available in [/legal](https://github.com/M1chol/SpaceGame/tree/master/legal)   
documentation avaiable in [/doc](https://github.com/M1chol/SpaceGame/tree/master/doc)   
   
Target os:
  1. Windows
  2. Linux

## Installing
For windows simply [download lates release](https://github.com/M1chol/SpaceGame/releases) - not ready yet
   
## Build from source
### Windows
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  (optional)
    
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
If you didn't install `mingw32-make` instead launch command flagged for `Windows_NT` from [Makefile](https://github.com/M1chol/SpaceGame/blob/master/Makefile) manually

### Linux - not a priority
take those instructions with a grain of salt. I didn't update them recently as I am developing this game on Windows machine. You are on your own Makefile won't work as it was not updated.

1. make
2. SDL2
3. SDL2_image
4. SDL2_ttf
   
Start by installing SDL for your system. You can use package menager or [build from surce](https://github.com/libsdl-org/SDL). You will also need SDL2_image and SDL2_ttf

__Arch__ `sudo pacman -S sdl2 sdl2_image`   
__Ubuntu__ `sudo apt-get install libsdl2 libsdl2-image` 
Then   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
make
```
