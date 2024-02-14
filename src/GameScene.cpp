#include "../inc/GameScene.hpp"
#include "../inc/Display.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/EntityHandler.hpp"
#include "../inc/Constants.hpp"
#include "../inc/NetworkHandler.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#include <windows.h>
#endif
#include <inttypes.h>

// TODO in multiplayer mode add lobby before game starts  to know amout of players.
void GameScene::Start(uint8_t numberOfPlayers)
{
    // Spawn players
    for (uint8_t iPlayer = 0; iPlayer < numberOfPlayers; iPlayer++)
    {
        auto newPlayer = _entityManager.AddEntity(ENTITY_TAG_PLAYER);
        Vec2f startPos{(float)(MAP_WIDTH/MAX_PLAYERS)/numberOfPlayers + iPlayer*MAP_WIDTH/MAX_PLAYERS, 
                       (float)(MAP_HEIGHT/MAX_PLAYERS)/numberOfPlayers + iPlayer*MAP_HEIGHT/MAX_PLAYERS};
        newPlayer->AddComponent<CTransform>(startPos, Vec2f(0, 0));
        newPlayer->AddComponent<CCannon>(0);
        newPlayer->AddComponent<CIndex>(iPlayer);
        newPlayer->AddComponent<CScore>();
    }

    bool clientUpdateNeeded = false;
    uint8_t countSinceLastClientUpdate = 0;

    // TODO: Bad ownership of entitiyHandler, should we use shared pointer or change ownership?
    // TODO _networkHandler.setEntetiesHandler(_entityHandler); // TODO: these are tightly coupled, could this be avoided?

    uint64_t currentTime = SDL_GetPerformanceCounter();
    uint64_t timerStart;
    uint64_t timerEnd;
    uint64_t timeTilNextFrame;
    uint64_t lastUpdateTime = currentTime;

    //While application is running
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        currentTime = SDL_GetPerformanceCounter();

        _entityManager.Update();
        SUserInput();

        if (_networkMode == NETWORK_MODE_CLIENT)
        {
            // TODO _networkHandler.C2SGameLoopActions(actions);
            // actions.PlayersShooting = 0;
        }

        if (_networkMode == NETWORK_MODE_SERVER)
        {
            std::vector<uint8_t> newActions = _networkHandler.ServerPollAllEvents();
            for (uint8_t &iAction : newActions)
            {
                printf("iAction: %d\n", iAction);
                // TODO
                // actions.PlayersShooting |= (iAction == NETWORK_ACTION_SHOOT_P1);
                // actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P2) << 1);
                // actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P3) << 2);
                // actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P4) << 3);
                // actions.PlayersShooting |= ((iAction == NETWORK_ACTION_SHOOT_P5) << 4);

                if (iAction == NETWORK_ACTION_NEW_CONNECTION)
                {
                    // Add a player
                    // TODO _entityHandler.AddPlayer();
                    clientUpdateNeeded = true;
                }
            }
        }
        // TODO if (actions.PlayersShooting > 0)
        // {
        //     clientUpdateNeeded = true;
        // }

        // Fixed rate updates up to current time, using GAME_UPDATE_TIME as refresh rate.
        uint8_t gameUpdatesCount = 0;
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

            SMovement();
            SPlayerActions();
            SCollisions();
            SCheckEndScore();
        }


        // Manage Network
        if (_networkMode == NETWORK_MODE_SERVER
            && (clientUpdateNeeded || countSinceLastClientUpdate > 250))
        {
            // TODO _networkHandler.S2CGameSnapshot(_entityHandler.GetGameSnapShot());
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

        _display->RenderAll(_entityManager);

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
