.PHONY: all clean

all: clean
ifeq ($(OS), Windows_NT)
	g++ -Iinclude -Iinclude/sdl -Iinclude/lua -Iinclude/headers -Llib -o bin/Main \
	src/*.cpp -llua54 -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image \
	-static-libgcc -static-libstdc++ -g
else
	g++ src/*.cpp -o bin/Main -Iinclude/headers -Iinclude/headers/prefabs \
	-Iinclude/headers/scenes -Iinclude/sdl -lSDL2main -lSDL2 -lSDL2_image \
	-lSDL2_ttf
endif

clean:
ifeq ($(OS), Windows_NT)
	if exist bin\Main.exe del /F bin\Main.exe
else
	rm -f bin/Main
endif