# General information
This project is Built on Simple DirectMedia Layer (Sam Lantinga slouken@libsdl.org)
   
## Thanks to
Mr. Foo' for providing great way to begin developing with SDL (https://lazyfoo.net/tutorials/SDL)   
Bastian Schiffthaler for creating cool logging utility (https://github.com/bschiffthaler/BSlogger)   
   
## Installing
__Windows__
1. __Use provided binary__ you can find latest exe file in releases   
2. __Build from source__ You will need these dependencies   
- [git](https://git-scm.com/download/win)
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  
   
then in powershell ecexute   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
You can also just execute lengthy command inside Makefile without downloading `mingw32-make`.
   
__Linux__   
Build from source, dependencies   
1. git
2. cmake
3. SDL
   
Start by installing SDL for your system. You can use package menager or [build from surce](https://github.com/libsdl-org/SDL). Here are some of distros:

__Arch__ `sudo pacman -S sdl2`
__Ubuntu__ `sudo apt-get install libsdl2`
Then

```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
cmake . -Bbuild
cmake --build build
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