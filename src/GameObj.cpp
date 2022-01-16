#include "../inc/GameObj.hpp"
#include "../inc/Display.hpp"
// #include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/EntityHandler.hpp"
#include "../inc/Constants.hpp"
#include "../inc/CollisionHandler.hpp"
#include "../inc/NetworkHandler.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
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
            printf( "Failed to load media!\n" );
        }
        else
        {	
            int input;
            NetworkHandler networkHandler{};
            EntityHandler entities{&networkHandler};
            networkHandler.setEntetiesHandler(&entities); // TODO: these are tightly coupled, could this be avoided?
            InputHandler inputHandler{};
            CollisionHandler collisionHandler{};

            //While application is running
            while( input != INPUT_QUIT )
            {
                input = inputHandler.EventHandler();
                if (input == INPUT_HOST)
                {
                    networkHandler.Host();
                }
                else if (input == INPUT_JOIN)
                {
                    networkHandler.Join();
                }
                else if (input == INPUT_DISCONNECT)
                {
                    networkHandler.Disconnect();
                }
                else if (input == INPUT_SEND_P2SHOOT)
                {
                    networkHandler.Shoot();
                }
                
                // TODO add check if is server or not.
                std::vector<uint8_t> recivedActions = networkHandler.PollAllServerEvents();
                for (std::vector<uint8_t>::iterator it = recivedActions.begin(); it != recivedActions.end(); ++it)
                {
                    if (*it == NETWORK_ACTION_SHOOT_P2) // TODO this needs to be moved somewhere else
                    {
                        entities.ServerUpdate(*it);
                    }
                }

                // Player point of view update
                entities.ServerUpdate(input);
                networkHandler.PollAllClientEvents();
                collisionHandler.HandleCollisons(entities);
                
                _Display.RenderAll(&entities);
                SDL_Delay(1000/60); // TODO: Add fixed game update time
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
