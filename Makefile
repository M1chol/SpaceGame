all:
ifeq ($(OS), Windows_NT)
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o Main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else
	g++ src/*.cpp -o game -Iinclude/headers -Iinclude/sdl -lSDL2
endif