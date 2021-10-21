#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/EntityHandler.hpp"
#include "../inc/Constants.hpp"
#include "../inc/CollisionHandler.hpp"
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
		//Load media
		if( !_Display.loadTextures() )
		{
			printf( "Failed to load _edia!\n" );
		}
		else
		{	
			int input;
			EntityHandler entities{};
			InputHandler inputHandler{};
			CollisionHandler collisionHandler{};

			//While application is running
			while( input != INPUT_QUIT )
			{
				input = inputHandler.EventHandler();
				entities.Update(input);
				collisionHandler.HandleCollisons(&entities);
				
				_Display.RenderAll(&entities);
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
