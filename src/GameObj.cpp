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
            printf("input: %i", input);
            if (input == INPUT_ESCAPE)
            {
                input = INPUT_NONE;
            }
        }
        if (input == INPUT_LOCAL_PLAY)
        {
            input = StartLocalPlay();
        }
        _display.RenderMainMenu();
    }
}

int GameObj::StartLocalPlay()
{
    InputHandler inputHandler{};
    int input = INPUT_NONE;
    int numberOfPlayers = 0;
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        input = inputHandler.EventHandler();
        if (input != INPUT_NONE)
        {
            switch (input)
            {
            case INPUT_2:
                numberOfPlayers = 2;
                break;
            case INPUT_3:
                numberOfPlayers = 3;
                break;
            case INPUT_4:
                numberOfPlayers = 4;
                break;
            case INPUT_5:
                numberOfPlayers = 5;
                break;
            case INPUT_6:
                numberOfPlayers = 6;
                break;
            default:
                printf("Not valid number of players!");
                continue; // return from the if case.
            }
            StartGameLoop(INPUT_LOCAL_PLAY);
            return 1; // Add next screen here.
        }
        _display.RenderLocalPlay();
    }
    return input;
}

bool SortBySec(const std::pair<uint8_t,uint8_t> &a, const std::pair<uint8_t,uint8_t> &b)
{
    return (a.second > b.second);
}

int GameObj::StartEndScore(std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc)
{
    InputHandler inputHandler{};
    int input = INPUT_NONE;
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        input = inputHandler.EventHandler();
        _display.RenderEndScore(playerAndScoreDesc);
    }
    return input;
}


int GameObj::StartGameLoop(int input)
{
    int8_t networkMode = -1;
    NetworkHandler networkHandler{};
    if (input == INPUT_HOST)
    {
        if (networkHandler.Host())
        {
            networkMode = NETWORK_MODE_SERVER;
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
            networkMode = NETWORK_MODE_CLIENT;
        }
        else
        {
            // Unsuccessful join, go back to main menu
            return input;
        }
    }
    else if (input == INPUT_LOCAL_PLAY)
    {
        networkMode = NETWORK_MODE_LOCAL;
    }
    else
    {
        printf("StartGameLoop::Incorrect input to start from!");
        return -1;
    }
    bool clientUpdateNeeded = false;
    uint8_t countSinceLastClientUpdate = 0;


    networkHandler.setEntetiesHandler(&_entityHandler); // TODO: these are tightly coupled, could this be avoided?
    InputHandler inputHandler{};
    std::vector<uint8_t> recivedActions;

    uint64_t currentTime = SDL_GetPerformanceCounter();
    uint64_t timerStart;
    uint64_t timerEnd;
    uint64_t timeTilNextFrame;
    uint64_t lastUpdateTime = currentTime;

    //While application is running
    while(input != INPUT_QUIT && input != INPUT_ESCAPE)
    {
        currentTime = SDL_GetPerformanceCounter();

        // Handle input
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
        if (input == INPUT_P1SHOOT || input == INPUT_P2SHOOT)
        {
            newActions.push_back(input);
        }

        // Append new actions to end of recived actions.
        if (!newActions.empty())
        {
            // printf("NewActions length: %i\n", newActions.size());
            recivedActions.insert(recivedActions.end(), newActions.begin(), newActions.end());
            // printf("RecivedActions length: %i\n", recivedActions.size());
        }

        // TODO convert recived actions to GAMELOOP_ACTIONS
        GAMELOOP_ACTIONS actions = {.PlayersShooting = 0};
        for (auto &&iAction : recivedActions)
        {
            printf("iAction: %d\n", iAction);
            actions.PlayersShooting |= (iAction == INPUT_P1SHOOT);
            actions.PlayersShooting |= (iAction == NETWORK_ACTION_SHOOT_P1);
            actions.PlayersShooting |= ((iAction == INPUT_P2SHOOT) << 1);
            actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P2) << 1);
        }
        // printf("P1 shooting:%d\n", actions.PlayersShooting);
        recivedActions.clear();

        // Fixed rate updates up to current time, using GAME_UPDATE_TIME as refresh rate.
        GAMELOOP_OUTPUT output =  GameLoop(actions, currentTime, lastUpdateTime);
        lastUpdateTime = output.lastUpdateTime;
        if (output.playerWon)
        {
            std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc;
            for (uint8_t iPlayer = 1; iPlayer <= MAX_PLAYERS; iPlayer++) // TODO instead use the connected amount of players?
            {
                playerAndScoreDesc.push_back(std::make_pair(iPlayer,_entityHandler.GetPlayerScore(iPlayer)));
            }
            std::sort(playerAndScoreDesc.begin(), playerAndScoreDesc.end(), SortBySec);
            input = StartEndScore(playerAndScoreDesc);
            return input;
        }

        // Manage Network
        if (networkMode == NETWORK_MODE_SERVER
            && (clientUpdateNeeded || countSinceLastClientUpdate > 250))
        {
            networkHandler.S2CGameSnapshot(_entityHandler.GetGameSnapShot());
            clientUpdateNeeded = false;
            countSinceLastClientUpdate = 0;
        }
        else if (networkMode == NETWORK_MODE_SERVER)
        {
            countSinceLastClientUpdate++;
        }
        if (networkMode == NETWORK_MODE_CLIENT)
        {
            networkHandler.PollAllClientEvents();
        }

        _display.RenderAll(&_entityHandler);

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

GAMELOOP_OUTPUT GameObj::GameLoop(GAMELOOP_ACTIONS actions, uint64_t currentTime, uint64_t lastUpdateTime)
{
    uint8_t gameUpdatesCount = 0;
    GAMELOOP_OUTPUT output{.playerWon = false};
    // Fixed rate updates:
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
        _entityHandler.MoveAllObjects();

        // Handle actions
        _entityHandler.HandlePlayerActions(actions);

        // Handle collisions
        _collisionHandler.HandleCollisons(_entityHandler);

        // Check if anyone has won
        // TODO: Handle the sorting and stuff somewhere else!?
        for (uint8_t iPlayer = 1; iPlayer <= MAX_PLAYERS; iPlayer++) // TODO instead use the connected amount of players?
        {
            if (_entityHandler.GetPlayerScore(iPlayer) >= END_SCORE)
            {
                // We have a winner!
                GAMELOOP_OUTPUT output {.playerWon = true};
                return output;
            }
        }
    }
    output.lastUpdateTime = lastUpdateTime;
    return output;
}
