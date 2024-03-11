# General information
This project is Built on Simple DirectMedia Layer (Sam Lantinga slouken@libsdl.org)
   
## Thanks to
Mr. Foo' for providing great way to begin developing with SDL (https://lazyfoo.net/tutorials/SDL)   
Bastian Schiffthaler for creating cool logging utility (https://github.com/bschiffthaler/BSlogger)   
   
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
If you didnt install `mingw32-make` instead launch   
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


## Stucture of the project
`src` - place for cpp files, all cpp files included here will be compiled to exe   
`include` - files to be included to compilations   
`include/headers` - header files for the project   
`include/sdl` - header files of SDL library   
`lib` - SDL files   
`res` - resources like images and sounds
`legal` - place for license files   
   
Misc   
`.gitignore` - gitignore file   
`COPYING.txt` - file requested by SDL library   
`LICENSE.txt` - license file of the project   
`Makefile` - make file of the project 
`NOTICE.txt` - notice file for license   
   
 Michał Kozłowski 2024
