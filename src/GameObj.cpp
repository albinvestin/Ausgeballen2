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
            int8_t isServer = -1;
            bool clientUpdateNeeded = false;
            uint8_t countSinceLastClientUpdate = 0;
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
                    isServer = networkHandler.Host();
                }
                else if (input == INPUT_JOIN)
                {
                    isServer = !(networkHandler.Join());
                }
                else if (input == INPUT_DISCONNECT)
                {
                    networkHandler.Disconnect();
                }
                else if (input == INPUT_SEND_P2SHOOT)
                {
                    networkHandler.Shoot(2);
                }

                // Move all objects first.
                entities.MoveAllObjects();

                if (isServer == 1)
                {
                    std::vector<uint8_t> recivedActions = networkHandler.PollAllServerEvents();
                    for (std::vector<uint8_t>::iterator it = recivedActions.begin(); it != recivedActions.end(); ++it)
                    {
                        // TODO These ifs are unnecessary since both cases are tested inside ServerCheckAndHandleShoot.
                        if (*it == NETWORK_ACTION_SHOOT_P2) // TODO this needs to be moved somewhere else
                        {
                            entities.ServerCheckAndHandleShoot(*it);
                            clientUpdateNeeded = true;
                        }
                    }
                    // TODO These ifs are unnecessary since both cases are tested inside ServerCheckAndHandleShoot.
                    if (input == INPUT_P1SHOOT) // TODO this needs to be moved somewhere else
                    {
                        entities.ServerCheckAndHandleShoot(input);
                        clientUpdateNeeded = true;
                    }
                }

                collisionHandler.HandleCollisons(entities);

                
                if (isServer == 1
                    && (clientUpdateNeeded || countSinceLastClientUpdate > 10))
                {
                    entities.UpdateClients();
                    clientUpdateNeeded = false;
                    countSinceLastClientUpdate = 0;
                }
                else
                {
                    countSinceLastClientUpdate++;
                }
                if (isServer == 0)
                {
                    networkHandler.PollAllClientEvents();
                }
                
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
