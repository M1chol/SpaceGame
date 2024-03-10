#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "logger.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
enum Keytest{
	playerUp,
	playerRight,
	playerDown,
	playerLeft,
	playerDef,
	keysNr
};
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* playerSurfaceList[keysNr];
SDL_Surface* playerSurface = NULL;

// load all media for player
bool loadPlayerMedia();

// start SDL and create game window
bool init();

// free resources and close app
void close();

// load surface at specified path
SDL_Surface* loadSurface(string);


int main( int argc, char* args[] )
{
	LOG_INIT_CERR();
	if (!init()){
		log(LOG_ERR) << "Game failed during initialization\n" << SDL_GetError();
		close();
	}else{
		log(LOG_INFO) << "Initialization successful\n";
		bool quit = false;
		SDL_Event e;

		// MAIN LOOP
		log(LOG_INFO) << "Setup finished starting game loop\n";
		while(!quit){
			// Check all events
			while(SDL_PollEvent(&e) != 0){
				if(e.type == SDL_QUIT){
					quit = true;
					log(LOG_INFO) << "Quit requested exiting loop\n";
				}else if (e.type = SDL_KEYDOWN){
					// keyboard check
					switch (e.key.keysym.sym){
						case SDLK_w:
							playerSurface = playerSurfaceList[playerUp];
							break;
						case SDLK_a:
							playerSurface = playerSurfaceList[playerLeft];
							break;
						case SDLK_s:
							playerSurface = playerSurfaceList[playerDown];
							break;
						case SDLK_d:
							playerSurface = playerSurfaceList[playerRight];
							break;
						default:
							playerSurface = playerSurfaceList[playerDef];
					}
				}
			}
			SDL_BlitSurface(playerSurface, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
		}
	}
	close();
	return 0;
}

bool init(){
	LOG_INIT_CERR();
	bool success = true;
	if ( SDL_Init(SDL_INIT_VIDEO) < 0){
		log(LOG_ERROR) << "SDL could not initialize successfuly " << SDL_GetError() << "\n";
		success = false;
	} else {
		// Create window
		gWindow = SDL_CreateWindow("Game",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			log(LOG_ERR) << "Failed creating window " << SDL_GetError() << "\n";
			success = false;
		}
		gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	if(!loadPlayerMedia()){
		log(LOG_WARN) << "Player media failed to load\n";
		success = false;
	}
	return success;
}

bool loadPlayerMedia(){
	LOG_INIT_CERR();
	bool success = true;
	playerSurfaceList[playerUp] = loadSurface("res/player-up.bmp");
	playerSurfaceList[playerDown] = loadSurface("res/player-down.bmp");
	playerSurfaceList[playerLeft] = loadSurface("res/player-left.bmp");
	playerSurfaceList[playerRight] = loadSurface("res/player-right.bmp");
	playerSurfaceList[playerDef] = loadSurface("res/player-placeholder.bmp");
	for(const auto& image: playerSurfaceList){
		if(image == NULL){
			return false;
		}
	}
	return true;
}

void close(){
	// Dealocate surface
	for (int i = 0; i < keysNr; ++i) {
		SDL_FreeSurface(playerSurfaceList[i]);
	}
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	playerSurface = NULL;
	SDL_Quit();
}

SDL_Surface* loadSurface(string path){
	LOG_INIT_CERR();
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	SDL_Surface* optimized = NULL;
	if(surface == NULL){
		log(LOG_WARN) << "Media failed to load (" << path << ") " << SDL_GetError() << "\n";
	}else{
		optimized = SDL_ConvertSurface(surface, gScreenSurface->format, 0);
		if(optimized == NULL){
			log(LOG_WARN) << "Media failed to optimize (" <<  path << ") " << SDL_GetError() << "\n";
		}
	}
	SDL_FreeSurface(surface);
	return optimized;
}