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
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* HelloWorld = NULL;

bool loadMedia();
bool init();
void close();

int main( int argc, char* args[] )
{
	LOG_INIT_CERR();
	if (!init()){
		log(LOG_ERR) << "Game failed during initialization\n"; 
	}else{
		if (!loadMedia()){
			log(LOG_WARN) << "Media failed loading\n";
		}else{
			SDL_BlitSurface(HelloWorld, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	bool quit = false;
	SDL_Event e;

	// MAIN LOOP
	while(!quit){
		// Check all events
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
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
	return success;
}

bool loadMedia(){
	LOG_INIT_CERR();
	bool success = true;
	HelloWorld = SDL_LoadBMP("res/player-placeholder.bmp");
	if(HelloWorld == NULL){
		log(LOG_WARN) << "Media could not be loaded " << SDL_GetError() << "\n";
		success = false;
	}
	return success;
}

void close(){
	// Dealocate surface
	SDL_FreeSurface(HelloWorld);
	HelloWorld = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}