#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
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
	if( !gDisp.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !gDisp.loadTextures() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

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
				gDisp.RenderAll();
			}
		}
	}
}

void GameObj::close()
{
    gDisp.close();

	//Quit SDL subsystems
	SDL_Quit();
}
