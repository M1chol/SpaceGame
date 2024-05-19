all:
ifeq ($(OS), Windows_NT)
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Iinclude/headers/prefabs -Llib -o Main src/*.cpp src/prefabs/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else
	g++ src/*.cpp src/prefabs/*.cpp -o Main -Iinclude/headers -Iinclude/headers/prefabs -Iinclude/sdl -lSDL2main -lSDL2 -lSDL2_image
endif