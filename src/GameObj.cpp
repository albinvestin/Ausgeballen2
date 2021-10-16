#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/EntityHandler.hpp"
#if defined(__APPLE__) || defined(__linux__)
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
	if( !_Display.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load _edia
		if( !_Display.loadTextures() )
		{
			printf( "Failed to load _edia!\n" );
		}
		else
		{	
			int input;
			int quit = 1; // TODO: _ake a DEFINE.
			
			EntityHandler entities{};
			//Player p1{};
			InputHandler inputHandler{};

			//While application is running
			while( input != quit )
			{
				input = inputHandler.EventHandler();
				entities.Update(input);
				
				_Display.RenderAll(entities);
				SDL_Delay(1000/60);
			}
		}
	}
}

void GameObj::close()
{
    _Display.close();

	//Quit SDL subsystems
	SDL_Quit();
}
