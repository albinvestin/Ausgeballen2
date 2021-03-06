#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include <SDL2/SDL.h>

GameObj::GameObj()
{
}

GameObj::~GameObj()
{
}

void GameObj::start()
{
    //Start up SDL and create window
	if( !mDisplay.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !mDisplay.loadTextures() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			Player p1{};

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}
				Vec2 p1Pos = p1.update();

				mDisplay.RenderAll(p1Pos);
			}
		}
	}
}

void GameObj::close()
{
    mDisplay.close();

	//Quit SDL subsystems
	SDL_Quit();
}
