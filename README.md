## development in early stages

license files available in /legal   
documentation avaiable in /doc

Source-avaiable Space Invaders style c++ game based on SDL

for now there is no way to support project

Target os:
  1. Windows
  2. Linux

## Installing
__Windows__
1. __Use provided binary__ you can find latest exe file in releases   
2. __Build from source__ You will need these dependencies   
- [git](https://git-scm.com/download/win)
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  
   
then in powershell execute   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
You can also just execute lengthy command inside Makefile without downloading `mingw32-make`.
   
__Linux__   
Build from source, dependencies   
2. cmake
3. SDL
   
Start by installing SDL for your system. You can use package menager or [build from surce](https://github.com/libsdl-org/SDL). Here are some of distros:

__Arch__ `sudo pacman -S sdl2 cmake`
__Ubuntu__ `sudo apt-get install libsdl2` `sudo apt install cmake`
Then

```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
cmake . -Bbuild
cmake --build build
```
