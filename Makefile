# Makefile for Space game
# last update: 03.07.2025

# Compiler and flags
CXX := g++
CXXFLAGS := -g -Iinclude -Iinclude/sdl -Iinclude/lua -Iinclude/sol -Iinclude/headers
LDFLAGS := -Llib
LIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -g

# Source and output directories
SRC_DIR := src/engine
OUTPUT := bin/Main

# Dependency check for Linux
PKG := pkg-config
DEPS := sdl3 sdl2-compat SDL2_image SDL2_ttf lua5.3

.PHONY: all deps clean

# Default target
all: deps $(OUTPUT)

deps:
ifneq ($(OS),Windows_NT)
	@$(PKG) --print-errors --exists $(DEPS) \
		|| (echo >&2 "Some dependencies are missing" && exit 1)
endif

$(OUTPUT): $(SRC_DIR)/*.cpp
ifeq ($(PLATFORM),windows)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) -llua53 $(LDFLAGS)
else
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) -llua5.3 
endif

clean:
ifeq ($(OS), Windows_NT)
	if exist bin\Main.exe del /F bin\Main.exe
else
	rm -f bin/Main
endif



