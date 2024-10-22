**List of contents**
- [About](#about)
- [Installing](#installing)
- [Gallery](#gallery)

---

## About
This is my source-avaiable c++ game made with SDL library. As of now the game does not hava a proper name. Feel free to fork / modify / mod the game.

all license files available in [/legal](https://github.com/M1chol/SpaceGame/tree/master/legal)   
documentation avaiable in [/doc](https://github.com/M1chol/SpaceGame/tree/master/doc)   
   
Target os:
  1. Windows
  2. Linux
  3. MacOS (???)

## Installing
**Windows** - [download (not ready)](https://github.com/M1chol/SpaceGame/releases) or build from source   
**Linux** - build from source   
**MacOS** - build from source   

To build from source follow lower instructions   

#### Windows
- [MinGW](https://sourceforge.net/projects/mingw/) 
- mingw32-make  (optional)
    
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
mingw32-make.exe
```
If you didn't install `mingw32-make` instead launch command flagged for `Windows_NT` from [Makefile](https://github.com/M1chol/SpaceGame/blob/master/Makefile) manually

#### Linux - not a priority
**Warning** I am developing this game on Windows machine, so Makefile for Linux could be outdated (Last update 22.10.2024). Lower you can find dependencied for **Debian**. for other distros search for the packages in parentaccess.

1. make
2. libsdl2-dev (SDL2)
3. libsdl2-image-dev (SDL2_image)
4. libsdl2-ttf-dev (SDL2_ttf)

`sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`   

Then   
```
git clone https://github.com/M1chol/SpaceGame
cd SpaceGame
make
```
#### MacOS
Ok, this should technicly work, as I do not own any Apple devices I do not care enough check it. You will need same dependencies as for linux, but I do not have a MakeFile for MacOS so you are on your own. 

---
## Gallery
**Not** in-game footage just dev visualizations  
  
![iamge](https://github.com/M1chol/SpaceGame/blob/master/doc/img/viz1.png)
![image](https://github.com/M1chol/SpaceGame/blob/master/doc/img/viz2.png)
![image](https://github.com/M1chol/SpaceGame/blob/master/doc/img/viz3.png)
