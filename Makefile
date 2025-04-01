.PHONY: all clean

all: clean
ifeq ($(OS), Windows_NT)
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Iinclude/lua -Iinclude/headers/prefabs -Iinclude/headers/scenes -Llib \
	-o bin/Main src/*.cpp src/prefabs/*.cpp src/scenes/*.cpp -lmingw32 -lSDL2main -lSDL2 \
	-lSDL2_ttf -lSDL2_image -llua54 -static-libgcc -static-libstdc++ -g
else
	g++ src/*.cpp src/prefabs/*.cpp src/scenes/*.cpp -o bin/Main -Iinclude/headers -Iinclude/lua \
	-Iinclude/headers/prefabs -Iinclude/headers/scenes -Iinclude/sdl -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -llua54
endif