#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL_image.h>
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
//SDL_Surface* playerSurfaceList[keysNr];
//SDL_Surface* playerSurface = NULL;
SDL_Texture* playerTextureList[keysNr];
SDL_Texture* playerTexture = NULL;
SDL_Renderer* gRenderer = NULL;



// load all media for player
bool loadPlayerMedia();

// start SDL and create game window
bool init();

// free resources and close app
void close();

// load surface at specified path
SDL_Surface* loadSurface(string);

// load image from path as teture
SDL_Texture* loadTexture(string);

// draw rectangle at x,y and with width and height
void renderRect(float, float, float, float);

int main( int argc, char* args[] )
{
	LOG_INIT_CERR();
	if (!init()){
		log(LOG_ERR) << "Game failed during initialization\n" << SDL_GetError();
		close();
	}else{
		log(LOG_INFO) << "Initialization successful\n";

		if(!loadPlayerMedia()){
			log(LOG_WARN) << "Player media failed to load\n";
		return false;
	}

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
							playerTexture = playerTextureList[playerUp];
							break;
						case SDLK_a:
							playerTexture = playerTextureList[playerLeft];
							break;
						case SDLK_s:
							playerTexture = playerTextureList[playerDown];
							break;
						case SDLK_d:
							playerTexture = playerTextureList[playerRight];
							break;
						default:
							playerTexture = playerTextureList[playerDef];
					}
				}
			}
			SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, playerTexture, NULL, NULL);
			renderRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 50, 50);
			SDL_RenderPresent(gRenderer);
		}
	}
	close();
	return 0;
}

bool init(){
	LOG_INIT_CERR();

	// Try Init
	if ( SDL_Init(SDL_INIT_VIDEO) < 0){
		log(LOG_ERROR) << "SDL could not initialize successfuly " << SDL_GetError() << "\n";
		return false;
	}
	// Try to create window
	gWindow = SDL_CreateWindow("Game",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(gWindow == NULL){
		log(LOG_ERR) << "Failed creating window " << SDL_GetError() << "\n";
		return false;
	}
	//Try to create renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL){
		log(LOG_ERR) << "Failed to create renderer " << SDL_GetError() << "\n";
		return false;
	}

	//Try to init SDL_Image
	int imageFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imageFlags) & imageFlags)){
		log(LOG_ERR) << "SDL_Image failed to initialize " << IMG_GetError() << "\n";
		return false;
	}
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

bool loadPlayerMedia(){
	bool success = true;
	playerTextureList[playerDown] = loadTexture("res/player-down.png");
	playerTextureList[playerUp] = loadTexture("res/player-up.png");
	playerTextureList[playerLeft] = loadTexture("res/player-left.png");
	playerTextureList[playerRight] = loadTexture("res/player-right.png");
	playerTextureList[playerDef] = loadTexture("res/player-placeholder.png");
	for(const auto& image: playerTextureList){
		if(image == NULL){
			return false;
		}
	}
	return true;
}

void close(){
	// Dealocate surface
	for (int i = 0; i < keysNr; ++i) {
		SDL_DestroyTexture(playerTextureList[i]);
	}
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	playerTexture = NULL;
	SDL_Quit();
}

SDL_Surface* loadSurface(string path){
	LOG_INIT_CERR();
	SDL_Surface* surface = IMG_Load(path.c_str());
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

SDL_Texture* loadTexture(string path){
	LOG_INIT_CERR();
	SDL_Texture* texture = NULL;
	SDL_Surface* loaded = IMG_Load(path.c_str());
	if(loaded == NULL){
		log(LOG_WARN) << "Failed loading media (" << path << ") " << IMG_GetError() << "\n";
		return NULL;
	}
	texture = SDL_CreateTextureFromSurface(gRenderer, loaded);
	if(texture == NULL){
		log(LOG_WARN) << "Failed converting media to texture (" << path << ") " << IMG_GetError() << "\n";
		return NULL;
	}
	SDL_FreeSurface(loaded);
	return texture;
}

void renderRect(float x, float y, float width, float height){
	SDL_Rect rect = {x, y, width, height};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &rect);
}