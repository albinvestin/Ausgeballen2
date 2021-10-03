#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

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
			int input;
			int quit = 1; // TODO: make a DEFINE.
			int player1Key = 2; // TODO; make DEFINE
			
			Player p1{};
			InputHandler inputHandler{};
			Vec2 p1Pos;

			//While application is running
			while( input != quit )
			{
				input = inputHandler.EventHandler();
				
				if (input == player1Key)
				{
					p1Pos = p1.update();
				}

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
