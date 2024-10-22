all:
ifeq ($(OS), Windows_NT)
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Iinclude/headers/prefabs -Llib -o bin/Main src/*.cpp src/prefabs/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -static-libgcc -static-libstdc++ -g
else
	g++ src/*.cpp src/prefabs/*.cpp -o bin/Main -Iinclude/headers -Iinclude/headers/prefabs -Iinclude/sdl -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
endif