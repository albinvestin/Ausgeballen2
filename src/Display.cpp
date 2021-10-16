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
	_BallTexture.ModifyColor(255, 255, 255);
	std::vector<Vec2f> AllBulletPos = entities.GetAllBulletPos();
	std::vector<Vec2f>::iterator it = AllBulletPos.begin();
    while (it != AllBulletPos.end())
    {
		_BallTexture.render( (*it).x, (*it).y, _Renderer );
        ++it;
    }

	//Vec2 bulletPos = entities.GetBullet1Pos();
	//_HearthTexture.render( bulletPos.x, bulletPos.y, _Renderer );

    //Render Player1 to the screen
	Vec2f playerPos = entities.GetPlayerPos(1);
	_BallTexture.ModifyColor(50, 255, 255);
    _BallTexture.render(playerPos.x, playerPos.y, _Renderer);

	// Render player1 shooting direction
	SDL_SetRenderDrawColor(_Renderer, 0, 0, 255, 150); // BLUE
	float angle = entities.GetP1Aim();
	int centerX = playerPos.x+PLAYER_RADIUS;
	int centerY = playerPos.y+PLAYER_RADIUS;
	SDL_RenderDrawLine(_Renderer, centerX, centerY, centerX + cos(angle)*(2*PLAYER_RADIUS), centerY + sin(angle)*(2*PLAYER_RADIUS));

	//Render Player2 to the screen
	playerPos = entities.GetPlayerPos(2);
	// printf("P2: (%f,%f)\n",playerPos.x, playerPos.y);
	_BallTexture.ModifyColor(255, 50, 255);
    _BallTexture.render(playerPos.x, playerPos.y, _Renderer);

	//Render map borders
	SDL_SetRenderDrawColor(_Renderer, 0, 0, 0, 255); // Black
	SDL_RenderDrawLine(_Renderer, 0, 0, MAP_WIDTH, 0); // Top
	SDL_RenderDrawLine(_Renderer, 0, MAP_HEIGHT, MAP_WIDTH, MAP_HEIGHT); // Bottom
	SDL_RenderDrawLine(_Renderer, 0, 0, 0, MAP_HEIGHT); // Left
	SDL_RenderDrawLine(_Renderer, MAP_WIDTH, 0, MAP_WIDTH, MAP_HEIGHT); // Right


    //Update screen
    SDL_RenderPresent( _Renderer );
}
