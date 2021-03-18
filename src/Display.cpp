#include "../inc/Display.hpp"
#include "../inc/Texture.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
// extern SDL_Renderer* gRenderer;
// extern SDL_Window* gWindow;
// extern LTexture gFooTexture;
// extern LTexture gBackgroundTexture;

Display::Display()
{
}

Display::~Display()
{
}

bool Display::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool Display::loadTextures()
{
	//Loading success flag
	bool success = true;

	//Load Foo' texture
	if( !gFooTexture.loadFromFile( "../res/dot.bmp", gRenderer ) )
	{
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}
	
	//Load background texture
	if( !gBackgroundTexture.loadFromFile( "../res/heart.png", gRenderer ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}

	return success;
}

void Display::close()
{
    gFooTexture.free();
	gBackgroundTexture.free();

    SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;
    SDL_DestroyWindow( gWindow );
	gWindow = NULL;

    // Quit SDL2_image system
    IMG_Quit();
}

void Display::RenderAll()
{
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    //Render background texture to screen
    gBackgroundTexture.render( 0, 0, gRenderer );

    //Render Foo' to the screen
    gFooTexture.render( 240, 190, gRenderer );

    //Update screen
    SDL_RenderPresent( gRenderer );
}
