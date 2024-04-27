#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL_image.h>
#include "logger.h"
#include "gComponents.h"

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
SDL_Texture* playerTextureList[keysNr];
SDL_Texture* playerTexture = NULL;
SDL_Renderer* gRenderer = NULL;

gObject player;

// load all media for player
bool loadPlayerMedia();

// start SDL and create game window
bool init();

// free resources and close app
void close();

// draw rectangle at x,y and with width and height
void renderRect(int, int, int, int);

int main( int argc, char* args[] )
{	
	// SETUP
	LOG_INIT_CERR();
	if (!init()){
		log(LOG_ERR) << "Game failed during initialization\n" << SDL_GetError();
		close();
		return 0;
	}
	log(LOG_INFO) << "Initialization successful\n";

	bool quit = false;
	SDL_Event e;

	player.addComponent(new SpriteComponent(gRenderer, "res/player-placeholder.png"));
	printf("TEST2");

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
						break;
					case SDLK_a:
						break;
					case SDLK_s:
						break;
					case SDLK_d:
						break;
					default:
						break;
				}
			}
		}
		SDL_RenderClear(gRenderer);
		
		static_cast<SpriteComponent*>(player.getComponent(0))->simpleRender(toiVect(player.pos));

		SDL_RenderPresent(gRenderer);
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

// bool loadPlayerMedia(){
// 	bool success = true;
	
// 	playerTextureList[playerDown] = 
// 	playerTextureList[playerUp] = loadTexture("res/player-up.png");
// 	playerTextureList[playerLeft] = loadTexture("res/player-left.png");
// 	playerTextureList[playerRight] = loadTexture("res/player-right.png");
// 	playerTextureList[playerDef] = loadTexture("res/player-placeholder.png");
// 	for(const auto& image: playerTextureList){
// 		if(image == NULL){
// 			return false;
// 		}
// 	}
// 	return true;
// }

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


void renderRect(int x, int y, int width, int height){
	SDL_Rect rect = {x, y, width, height};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &rect);
}