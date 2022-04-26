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
#include <windows.h>
#endif
#include <inttypes.h>
GameObj::GameObj()
{
}

GameObj::~GameObj()
{
}

void GameObj::start()
{
    //Start up SDL and create window
    if( !_display.init() )
    {
        printf( "Failed to initialize!\n" );
        return;
    }

    //Load media
    if( !_display.loadTextures() )
    {
        printf( "Failed to load media!\n" );
        return;
    }

    int input;
    int8_t isServer = -1;
    bool clientUpdateNeeded = false;
    uint8_t countSinceLastClientUpdate = 0;
    NetworkHandler networkHandler{};
    EntityHandler entities{&networkHandler};
    networkHandler.setEntetiesHandler(&entities); // TODO: these are tightly coupled, could this be avoided?
    InputHandler inputHandler{};
    CollisionHandler collisionHandler{};
    std::vector<uint8_t> recivedActions;
// #if defined(_WIN32)
//             // Set timer resolution to 1ms for Sleep.
//             timeBeginPeriod(1);
// #endif


    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 lastUpdateTime = currentTime;
    uint8_t gameUpdatesCount = 0;

    //While application is running
    while( input != INPUT_QUIT )
    {
        
        currentTime = SDL_GetPerformanceCounter();
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

        // ONLY FOR SERVER: See if new network inputs have occured
        std::vector<uint8_t> newActions = networkHandler.PollAllServerEvents();
        if (input == INPUT_P1SHOOT)
        {
            newActions.push_back(INPUT_P1SHOOT);
        }

        // Append new actions to end of recived actions.
        if (!newActions.empty())
        {
            printf("NewActions length: %i\n", newActions.size());
            recivedActions.insert(recivedActions.end(), newActions.begin(), newActions.end());
            printf("RecivedActions length: %i\n", recivedActions.size());
        }

        // Fixed rate updates:
        gameUpdatesCount = 0;
        while ((currentTime - lastUpdateTime) >= GAME_UPDATE_TIME)
        {
            gameUpdatesCount++;
            if (gameUpdatesCount > 5)
            {
                printf("Clamping!\n");
                // Clamp the max allowed iterations per frame to not fall behind on spikes in framerate.
                gameUpdatesCount = 0;
                break;
            }
            // Fixed time update
            lastUpdateTime += GAME_UPDATE_TIME;

            // Move all objects
            // printf("Moving all objects.\n");
            entities.MoveAllObjects();

            // Handle queued actions
            if (isServer == 1)
            {
                for (std::vector<uint8_t>::iterator it = recivedActions.begin(); it != recivedActions.end(); ++it)
                {
                    // TODO These ifs are unnecessary since both cases are tested inside ServerCheckAndHandleShoot.
                    if (*it == NETWORK_ACTION_SHOOT_P2 || *it == INPUT_P1SHOOT) // TODO this needs to be moved somewhere else
                    {
                        entities.ServerCheckAndHandleShoot(*it);
                        clientUpdateNeeded = true;
                    }
                }
                recivedActions.clear();
                // TODO These ifs are unnecessary since both cases are tested inside ServerCheckAndHandleShoot.
                // if (input == INPUT_P1SHOOT) // TODO this needs to be moved somewhere else
                // {
                //     entities.ServerCheckAndHandleShoot(input);
                //     clientUpdateNeeded = true;
                // }
            }

            collisionHandler.HandleCollisons(entities);
        }

        // Manage Network
        if (isServer == 1
            && (clientUpdateNeeded || countSinceLastClientUpdate > 250))
        {
            entities.UpdateClients();
            clientUpdateNeeded = false;
            countSinceLastClientUpdate = 0;
        }
        else if (isServer == 1)
        {
            countSinceLastClientUpdate++;
        }
        if (isServer == 0)
        {
            networkHandler.PollAllClientEvents();
        }

        
        _display.RenderAll(&entities);
        // uint64_t timeTilNextFrame = lastUpdateTime + GAME_UPDATE_TIME - currentTime;
        // SDL_Delay(1);
        
        // return;
        // if (timeTilNextFrame*1000/SDL_GetPerformanceFrequency() > 4)
        // {
        //     SDL_Delay(timeTilNextFrame*1000/SDL_GetPerformanceFrequency());
        // }
        // else
        // {
        //     SDL_Delay(1);
        // }
        // SDL_Delay(1000/60); // TODO: Add fixed game update time
        // SDL_Delay(1);
        // printf("%f ms/frame\n", (double)((SDL_GetPerformanceCounter() - currentTime)*1000) / SDL_GetPerformanceFrequency());
        
    }
// #if defined(_WIN32)
//             // Free timer
//             timeEndPeriod(1);
// #endif
        

}

void GameObj::close()
{
    _display.close();

    //Quit SDL subsystems
    SDL_Quit();
}
 