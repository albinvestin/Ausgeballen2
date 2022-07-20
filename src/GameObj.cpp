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
#include <algorithm>

void GameObj::Start()
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

    StartMainMenu();
    Close();
}

void GameObj::Close()
{
    _display.close();

    //Quit SDL subsystems
    SDL_Quit();
}

void GameObj::StartMainMenu()
{
    InputHandler inputHandler{};
    int input = INPUT_NONE;
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        input = inputHandler.EventHandler();
        if (input == INPUT_HOST || input == INPUT_JOIN)
        {
            input = StartGameLoop(input);
            if (input == INPUT_ESCAPE)
            {
                input = INPUT_NONE;
            }
        }
        _display.RenderMainMenu();
    }
}

bool SortBySec(const std::pair<uint8_t,uint8_t> &a, const std::pair<uint8_t,uint8_t> &b)
{
    return (a.second > b.second);
}

void GameObj::StartEndScore(std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc)
{
    InputHandler inputHandler{};
    int input = INPUT_NONE;
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        input = inputHandler.EventHandler();
        _display.RenderEndScore(playerAndScoreDesc);
    }
}


int GameObj::StartGameLoop(int input)
{
    int8_t isServer = -1;
    NetworkHandler networkHandler{};
    if (input == INPUT_HOST)
    {
        if (networkHandler.Host())
        {
            isServer = true;
        }
        else
        {
            // Unsuccessful host, go back to main menu
            return input;
        }
    }
    else if (input == INPUT_JOIN)
    {
        if (networkHandler.Join())
        {
            isServer = false;
        }
        else
        {
            // Unsuccessful join, go back to main menu
            return input;
        }
    }
    bool clientUpdateNeeded = false;
    uint8_t countSinceLastClientUpdate = 0;

    EntityHandler entities{&networkHandler};
    networkHandler.setEntetiesHandler(&entities); // TODO: these are tightly coupled, could this be avoided?
    InputHandler inputHandler{};
    CollisionHandler collisionHandler{};
    std::vector<uint8_t> recivedActions;

    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 timerStart;
    Uint64 timerEnd;
    Uint64 timeTilNextFrame;
    Uint64 lastUpdateTime = currentTime;
    uint8_t gameUpdatesCount = 0;

    //While application is running
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        currentTime = SDL_GetPerformanceCounter();
        input = inputHandler.EventHandler();

        if (input == INPUT_DISCONNECT)
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
            }

            collisionHandler.HandleCollisons(entities);

            // Check if anyone has won
            for (uint8_t iPlayer = 1; iPlayer < MAX_PLAYERS; iPlayer++) // TODO instead use the connected amount of players?
            {
                if (entities.GetPlayerScore(iPlayer) >= END_SCORE)
                {
                    // We have a winner!
                    std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc;
                    // Do a second loop if we won instead of saveing all the values each time.
                    for (uint8_t iPlayer = 1; iPlayer <= MAX_PLAYERS; iPlayer++) // TODO instead use the connected amount of players?
                    {
                        playerAndScoreDesc.push_back(std::make_pair(iPlayer,entities.GetPlayerScore(iPlayer)));
                    }
                    std::sort(playerAndScoreDesc.begin(), playerAndScoreDesc.end(), SortBySec);
                    StartEndScore(playerAndScoreDesc);
                    return input;
                }
            }
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

        // Sleep until roughly next frame
        timeTilNextFrame = lastUpdateTime + GAME_UPDATE_TIME - currentTime;
        double timeDuration = ((timeTilNextFrame)*1000) / SDL_GetPerformanceFrequency();
        if (timeDuration > 16)
        {
            printf("Should wait %f ms\n", timeDuration); // TODO when grabbing the window this time get set to a very large number?
        }

        timerStart = SDL_GetPerformanceCounter();
        if (timeDuration > 3 && timeDuration < 16)
        {
            SDL_Delay((Uint32)timeDuration);
        }
        timerEnd = SDL_GetPerformanceCounter();
        double timerDuration = ((timerEnd - timerStart)*1000) / SDL_GetPerformanceFrequency();
        if (timerDuration - timeDuration > 1)
        {
            printf("Overshoot with %f ms\n", timerDuration - timeDuration);
        }
    }
    return input;
}
