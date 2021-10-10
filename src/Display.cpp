#include "../inc/Display.hpp"
#include "../inc/Texture.hpp"
#include "../inc/Vector2.hpp"
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
		mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( mWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );
			if( mRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
	if( !mBallTexture.loadFromFile( "../res/dot.bmp", mRenderer ) )
	{
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}
	
	//Load background texture
	if( !mHearthTexture.loadFromFile( "../res/heart.png", mRenderer ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}

	return success;
}

void Display::close()
{
    mBallTexture.free();
	mHearthTexture.free();

    SDL_DestroyRenderer( mRenderer );
	mRenderer = NULL;
    SDL_DestroyWindow( mWindow );
	mWindow = NULL;

    // Quit SDL2_image system
    IMG_Quit();
}

void Display::RenderAll(EntityHandler entities)
{
    //Clear screen
    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( mRenderer );

    //Render background texture to screen
    // mHearthTexture.render( 0, 0, mRenderer );

	// Render bullets
	std::vector<Vec2f> AllBulletPos = entities.GetAllBulletPos();
	std::vector<Vec2f>::iterator it = AllBulletPos.begin();
    while (it != AllBulletPos.end())
    {
		mBallTexture.render( (*it).x, (*it).y, mRenderer );
        ++it;
    }

	//Vec2 bulletPos = entities.GetBullet1Pos();
	//mHearthTexture.render( bulletPos.x, bulletPos.y, mRenderer );

    //Render Foo' to the screen
	Vec2f playerPos = entities.GetP1Pos();
	float angle = entities.GetP1Aim();

    mBallTexture.render( playerPos.x, playerPos.y, mRenderer );

	// Render player1 shooting direction
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 150);

	int centerX = playerPos.x+10;
	int centerY = playerPos.y+10;

	SDL_RenderDrawLine(mRenderer, centerX, centerY, centerX + cos(angle)*20, centerY + sin(angle)*20);

    //Update screen
    SDL_RenderPresent( mRenderer );
}
