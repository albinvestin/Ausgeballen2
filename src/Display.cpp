#include "../inc/Display.hpp"
#include "../inc/Texture.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#elif defined(__linux)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif
#include <math.h>

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
		_Window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( _Window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			_Renderer = SDL_CreateRenderer( _Window, -1, SDL_RENDERER_ACCELERATED );
			if( _Renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( _Renderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
	if( !_BallTexture.loadFromFile( "../res/dot.bmp", _Renderer ) )
	{
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}
	
	//Load background texture
	if( !_HearthTexture.loadFromFile( "../res/heart.png", _Renderer ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}

	return success;
}

void Display::close()
{
    _BallTexture.free();
	_HearthTexture.free();

    SDL_DestroyRenderer( _Renderer );
	_Renderer = NULL;
    SDL_DestroyWindow( _Window );
	_Window = NULL;

    // Quit SDL2_image system
    IMG_Quit();
}

void Display::RenderAll(EntityHandler entities)
{
    //Clear screen
    SDL_SetRenderDrawColor( _Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( _Renderer );

    //Render background texture to screen
    // _HearthTexture.render( 0, 0, _Renderer );

	// Render bullets
	std::vector<Vec2f> AllBulletPos = entities.GetAllBulletPos();
	std::vector<Vec2f>::iterator it = AllBulletPos.begin();
    while (it != AllBulletPos.end())
    {
		_BallTexture.render( (*it).x, (*it).y, _Renderer );
        ++it;
    }

	//Vec2 bulletPos = entities.GetBullet1Pos();
	//_HearthTexture.render( bulletPos.x, bulletPos.y, _Renderer );

    //Render Foo' to the screen
	Vec2f playerPos = entities.GetP1Pos();
	float angle = entities.GetP1Aim();

    _BallTexture.render( playerPos.x, playerPos.y, _Renderer );

	// Render player1 shooting direction
	SDL_SetRenderDrawColor(_Renderer, 0, 0, 255, 150);

	int centerX = playerPos.x+10;
	int centerY = playerPos.y+10;

	SDL_RenderDrawLine(_Renderer, centerX, centerY, centerX + cos(angle)*20, centerY + sin(angle)*20);

    //Update screen
    SDL_RenderPresent( _Renderer );
}
