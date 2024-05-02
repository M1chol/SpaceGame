#define SDL_MAIN_HANDLED
#include "engine.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

// start SDL and create game window
bool init();

// free resources and close app
void close();

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

	gObject player;
	player.pos = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	player.addComponent(new SpriteComponent(gRenderer, "res/player-placeholder.png"));

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
		
		//static_cast<SpriteComponent*>(player.getComponent(0))->render(2);
		player.render();
		SDL_RenderPresent(gRenderer);
	}
	close();
	return 0;
}

// TODO: Rework to engine
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

// TODO: Rework to engine. track all gObjects on scene and remove them
void close(){
	// For now  no implementation
	SDL_Quit();
}