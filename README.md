## Space Game SDL Project
This is my source-avaiable c++ game based Space Invaders made with c++ and SDL library. Feel free to fork / modify / mod the game. Selling the game is prohibited as stated in [license](https://github.com/M1chol/SpaceGame/blob/master/LICENSE.txt).

all license files available in [/legal](https://github.com/M1chol/SpaceGame/tree/master/legal)   
documentation avaiable in [/doc](https://github.com/M1chol/SpaceGame/tree/master/doc)   
   
Target os:
  1. Windows
  2. Linux

## Installing
For windows simply [download lates release](https://github.com/M1chol/SpaceGame/releases)
   
__Build from source__   
Windows dependencies  
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  (optional)
    
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
If you didn't install `mingw32-make` instead launch command from [Makefile](https://github.com/M1chol/SpaceGame/blob/master/Makefile) manually

Linux dependencies      
1. git
2. make
3. SDL2
4. SDL2_image
   
Start by installing SDL for your system. You can use package menager or [build from surce](https://github.com/libsdl-org/SDL). You will also need SDL2_image

__Arch__ `sudo pacman -S sdl2 sdl2_image`   
__Ubuntu__ `sudo apt-get install libsdl2 libsdl2-image` __(NOT TESTED)__   
Then   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
make
```
