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
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        if (_inputFlags.HOST)
        {
            _networkMode = NETWORK_MODE_SERVER;
            StartGameLoop(1);
        }
        else if (_inputFlags.JOIN)
        {
            _networkMode = NETWORK_MODE_CLIENT;
            StartGameLoop(1);
        }
        if (_inputFlags.LOCAL_PLAY)
        {
            _networkMode = NETWORK_MODE_LOCAL;
            StartLocalPlay();
        }
        _display.RenderMainMenu();
    }
}

void GameObj::StartLocalPlay()
{
    int numberOfPlayers = 0;
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        if (_inputFlags.NUMBER2)
        {
            numberOfPlayers = 2;
        }
        else if (_inputFlags.NUMBER3)
        {
            numberOfPlayers = 3;
        }
        else if (_inputFlags.NUMBER4)
        {
            numberOfPlayers = 4;
        }
        else if (_inputFlags.NUMBER5)
        {
            numberOfPlayers = 5;
        }
        if (numberOfPlayers > 0)
        {
            // TODO instead handle the numbers of players in this loop instead of in the StartGameLoop one.
            StartGameLoop(numberOfPlayers);
            return;
        }
        _display.RenderLocalPlay();
    }
}

bool SortBySec(const std::pair<uint8_t,uint8_t> &a, const std::pair<uint8_t,uint8_t> &b)
{
    return (a.second > b.second);
}

void GameObj::StartEndScore(const std::vector<std::pair<uint8_t,uint8_t>> &playerAndScoreDesc)
{
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        _display.RenderEndScore(playerAndScoreDesc);
    }
}


void GameObj::StartGameLoop(const uint8_t numberOfPlayers)
{
    if (_networkMode == NETWORK_MODE_SERVER)
    {
        bool success = _networkHandler.Host();
        if (!success)
        {
            printf("Unsucessful hosting!\n");
            return;
        }
        _entityHandler.Init(numberOfPlayers);
    }
    else if (_networkMode == NETWORK_MODE_CLIENT)
    {
        printf("Trying to join default ip...\n");
        bool success = _networkHandler.Join();
        if (!success)
        {
            printf("Unsucessful joining!\n");
            return;
        }
        _entityHandler.Init(numberOfPlayers);
    }
    else if (_networkMode == NETWORK_MODE_LOCAL)
    {
        // TODO use numberOfPlayers here. 
        _entityHandler.Init(numberOfPlayers);
        printf("Starting local play with hardcoded number of players\n");
    }
    else
    {
        printf("StartGameLoop::Incorrect input to start from!");
        return;
    }
    bool clientUpdateNeeded = false;
    uint8_t countSinceLastClientUpdate = 0;

    // TODO: Bad ownership of entitiyHandler, should we use shared pointer or change ownership?
    _networkHandler.setEntetiesHandler(_entityHandler); // TODO: these are tightly coupled, could this be avoided?

    uint64_t currentTime = SDL_GetPerformanceCounter();
    uint64_t timerStart;
    uint64_t timerEnd;
    uint64_t timeTilNextFrame;
    uint64_t lastUpdateTime = currentTime;

    //While application is running
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        currentTime = SDL_GetPerformanceCounter();

        _inputHandler.EventHandler(_inputFlags);
        if (_inputFlags.DISCONNECT)
        {
            _networkHandler.Disconnect();
            printf("Disconnecting.\n");
            return;
        }

        GAMELOOP_ACTIONS actions = {};
        actions.PlayersShooting |= _inputFlags.P1SHOOT;
        actions.PlayersShooting |= _inputFlags.P2SHOOT << 1;
        actions.PlayersShooting |= _inputFlags.P3SHOOT << 2;
        actions.PlayersShooting |= _inputFlags.P4SHOOT << 3;
        actions.PlayersShooting |= _inputFlags.P5SHOOT << 4;
        if (_networkMode == NETWORK_MODE_CLIENT)
        {
            _networkHandler.C2SGameLoopActions(actions);
            actions.PlayersShooting = 0;
        }

        if (_networkMode == NETWORK_MODE_SERVER)
        {
            std::vector<uint8_t> newActions = _networkHandler.ServerPollAllEvents();
            for (uint8_t &iAction : newActions)
            {
                printf("iAction: %d\n", iAction);
                actions.PlayersShooting |= (iAction == NETWORK_ACTION_SHOOT_P1);
                actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P2) << 1);
                actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P3) << 2);
                actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P4) << 3);
                actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P5) << 4);

                if (iAction == NETWORK_ACTION_NEW_CONNECTION)
                {
                    // Add a player
                    _entityHandler.AddPlayer();
                    clientUpdateNeeded = true;
                }
            }
        }
        if (actions.PlayersShooting > 0)
        {
            clientUpdateNeeded = true;
        }

        // Fixed rate updates up to current time, using GAME_UPDATE_TIME as refresh rate.
        GAMELOOP_OUTPUT output = GameLoop(actions, currentTime, lastUpdateTime, _entityHandler.GetNumberOfPlayers());
        if (output.playerWon)
        {
            std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc;
            for (uint8_t iPlayer = 1; iPlayer <= _entityHandler.GetNumberOfPlayers(); iPlayer++) // TODO instead use the connected amount of players?
            {
                playerAndScoreDesc.push_back(std::make_pair(iPlayer,_entityHandler.GetPlayerScore(iPlayer)));
            }
            std::sort(playerAndScoreDesc.begin(), playerAndScoreDesc.end(), SortBySec);
            StartEndScore(playerAndScoreDesc);
            return;
        }

        // Manage Network
        if (_networkMode == NETWORK_MODE_SERVER
            && (clientUpdateNeeded || countSinceLastClientUpdate > 250))
        {
            _networkHandler.S2CGameSnapshot(_entityHandler.GetGameSnapShot());
            clientUpdateNeeded = false;
            countSinceLastClientUpdate = 0;
        }
        else if (_networkMode == NETWORK_MODE_SERVER)
        {
            countSinceLastClientUpdate++;
        }
        if (_networkMode == NETWORK_MODE_CLIENT)
        {
            _networkHandler.PollAllClientEvents();
        }

        _display.RenderAll(_entityHandler);

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
}

GAMELOOP_OUTPUT GameObj::GameLoop(const GAMELOOP_ACTIONS &actions, const uint64_t currentTime, uint64_t &lastUpdateTime, uint8_t numberOfPlayers)
{
    uint8_t gameUpdatesCount = 0;
    GAMELOOP_OUTPUT output{};
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
        for (uint8_t iPlayer = 1; iPlayer <= numberOfPlayers; iPlayer++) // TODO instead use the connected amount of players?
        {
            if (_entityHandler.GetPlayerScore(iPlayer) >= END_SCORE)
            {
                // We have a winner!
                output.playerWon = true;
                return output;
            }
        }
    }

    return output;
}
