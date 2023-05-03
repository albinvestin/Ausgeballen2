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
    // Spawn players
    for (uint8_t iPlayer = 0; iPlayer < numberOfPlayers; iPlayer++)
    {
        auto newPlayer = _entityManager.AddEntity(ENTITY_TAG_PLAYER);
        Vec2f startPos{(float)(MAP_WIDTH/MAX_PLAYERS)/2 + iPlayer*MAP_WIDTH/MAX_PLAYERS, 
                       (float)(MAP_HEIGHT/MAX_PLAYERS)/2 + iPlayer*MAP_HEIGHT/MAX_PLAYERS};
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
        GAMELOOP_OUTPUT output = GameLoop(currentTime, lastUpdateTime, numberOfPlayers);
        if (output.playerWon)
        {
            std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc;
            for (uint8_t iPlayer = 1; iPlayer <= numberOfPlayers; iPlayer++) // TODO instead use the connected amount of players?
            {
                // TODO playerAndScoreDesc.push_back(std::make_pair(iPlayer,_entityHandler.GetPlayerScore(iPlayer)));
            }
            std::sort(playerAndScoreDesc.begin(), playerAndScoreDesc.end(), SortBySec);
            StartEndScore(playerAndScoreDesc);
            return;
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

        _display.RenderAll(_entityManager);

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

GAMELOOP_OUTPUT GameObj::GameLoop(const uint64_t currentTime, uint64_t &lastUpdateTime, uint8_t numberOfPlayers)
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
        // _entityHandler.MoveAllObjects();
        SMovement();

        // Handle actions
        SPlayerActions();
        // _entityHandler.HandlePlayerActions(actions);

        // Handle collisions
        SCollisions();
        // _collisionHandler.HandleCollisons(_entityHandler);

        // Check if anyone has won
        // TODO: Handle the sorting and stuff somewhere else!?
        // TODO 
        // for (uint8_t iPlayer = 1; iPlayer <= numberOfPlayers; iPlayer++) // TODO instead use the connected amount of players?
        // {
        //     if (_entityHandler.GetPlayerScore(iPlayer) >= END_SCORE)
        //     {
        //         // We have a winner!
        //         output.playerWon = true;
        //         return output;
        //     }
        // }
    }

    return output;
}

void GameObj::SMovement() // TODO move this to separate class?
{
    auto & entities = _entityManager.GetAllEntities();
    for (auto &&entitiy : entities)
    {
        auto &cTransform = entitiy->GetComponent<CTransform>();
        if (cTransform.has)
        {
            cTransform.position += cTransform.velocity;
        }
        if (entitiy->GetTag() == ENTITY_TAG_PLAYER)
        {
            if (cTransform.position.x + 2*PLAYER_RADIUS > MAP_WIDTH) // Right side
            {
                // Move the amount past the wall back into the map.
                cTransform.position.x -= (cTransform.position.x + 2*PLAYER_RADIUS - MAP_WIDTH);
                cTransform.velocity.x = -cTransform.velocity.x;
            }
            else if (cTransform.position.x < 0) // Left side
            {
                cTransform.position.x -= cTransform.position.x;
                cTransform.velocity.x = -cTransform.velocity.x;
            }
            if (cTransform.position.y + 2*PLAYER_RADIUS > MAP_HEIGHT) // Bottom side
            {
                // Move the amount past the wall back into the map.
                cTransform.position.y -= (cTransform.position.y + 2*PLAYER_RADIUS - MAP_HEIGHT);
                cTransform.velocity.y = -cTransform.velocity.y;
            }
            else if (cTransform.position.y < 0) // Top side
            {
                cTransform.position.y -= cTransform.position.y;
                cTransform.velocity.y = -cTransform.velocity.y;
            }

            // Decrease velocity
            if (Vec2Length(cTransform.velocity) < 1)
            {
                cTransform.velocity.x = 0;
                cTransform.velocity.y = 0;
            }
            else
            {
                cTransform.velocity *= PLAYER_FRICTION;
            }

            // Move canon
            auto &cCannon = entitiy->GetComponent<CCannon>();
            cCannon.aimDirection += PLAYER_AIM_SPEED;
            if (cCannon.aimDirection > 2*M_PI)
            {
                cCannon.aimDirection = 0;
            }
        }
    }
}

void GameObj::SpawnBullet(float aimDirection, const Vec2f &position, uint8_t playerIndex)
{
    auto newBullet = _entityManager.AddEntity(ENTITY_TAG_BULLET);
    newBullet->AddComponent<CTransform>(position, Vec2f((float)(BULLET_INIT_VEL * cos(aimDirection)),
                                                       (float)(BULLET_INIT_VEL * sin(aimDirection))));
    newBullet->AddComponent<CIndex>(playerIndex);
}

void GameObj::SPlayerActions()
{
    auto &players = _entityManager.GetEntities(ENTITY_TAG_PLAYER);
    for (auto &&player : players)
    {
        auto &cCannon = player->GetComponent<CCannon>();
        if (cCannon.isShooting)
        {
            auto &cTransform = player->GetComponent<CTransform>();
            auto &cIndex = player->GetComponent<CIndex>();
            cCannon.isShooting = false;
            SpawnBullet(cCannon.aimDirection, cTransform.position, cIndex.playerIndex);
            cTransform.velocity.x -= (cos(cCannon.aimDirection) * PLAYER_RECOIL);
            cTransform.velocity.y -= (sin(cCannon.aimDirection) * PLAYER_RECOIL);
        }
    }
}

void GameObj::SUserInput()
{
    _inputHandler.EventHandler(_inputFlags);
    if (_inputFlags.DISCONNECT)
    {
        _networkHandler.Disconnect();
        printf("Disconnecting.\n");
        _inputFlags.QUIT = true;
        return;
    }

    // set the cannon of the correct player to shooting
    auto &players = _entityManager.GetEntities(ENTITY_TAG_PLAYER);
    for (auto &&player : players)
    {
        auto &cIndex = player->GetComponent<CIndex>();
        auto &cCannon= player->GetComponent<CCannon>();
        switch (cIndex.playerIndex)
        {
        case 0:
            cCannon.isShooting = _inputFlags.P1SHOOT;
            break;
        case 1:
            cCannon.isShooting = _inputFlags.P2SHOOT;
            break;
        case 2:
            cCannon.isShooting = _inputFlags.P3SHOOT;
            break;
        case 3:
            cCannon.isShooting = _inputFlags.P4SHOOT;
            break;
        case 4:
            cCannon.isShooting = _inputFlags.P5SHOOT;
            break;
        
        default:
            break;
        }
    }
}

bool CollisionTwoCircles(const Vec2f &pos1, int radius1, const Vec2f &pos2, int radius2)
{
    Vec2f difference{pos2.x - pos1.x, pos2.y - pos1.y};
    return (Vec2Length(difference) <= radius1 + radius2);
}

float Squaredf(const float x)
{
    return x*x;
}

Vec2f ElasticCollision(const Vec2f &playerPos1, const Vec2f &playerPos2, const Vec2f &p1Vel, const Vec2f &p2Vel)
{
    Vec2f velDiff1 = p1Vel - p2Vel;
    Vec2f posDiff1 = playerPos1 - playerPos2;
    float dotProd1 = DotProduct(velDiff1, posDiff1);
    float squaredNorm1 = Squaredf(Vec2Length(posDiff1));
    Vec2f nominator1 = dotProd1 * posDiff1;

    Vec2f p1NewVel = p1Vel - (nominator1 / squaredNorm1);

    return p1NewVel;
}

void GameObj::SCollisions()
{
    // HandlePlayerOnBulletCollisions(entities);
    auto &players = _entityManager.GetEntities(ENTITY_TAG_PLAYER);
    auto &bullets = _entityManager.GetEntities(ENTITY_TAG_BULLET);

    for (auto &&player : players)
    {
        auto &cTransformPlayer = player->GetComponent<CTransform>();
        auto &cIndexPlayer = player->GetComponent<CIndex>();
        for (auto &&bullet : bullets)
        {
            auto &cTransformBullet = bullet->GetComponent<CTransform>();
            auto &cIndexBullet = bullet->GetComponent<CIndex>();
            if (cIndexPlayer.playerIndex != cIndexBullet.playerIndex
                && CollisionTwoCircles(cTransformPlayer.position, PLAYER_RADIUS, cTransformBullet.position, BULLET_RADIUS))
            {
                _entityManager.RemoveEntity(*bullet);
                player->GetComponent<CScore>().score++;
                cTransformPlayer.velocity += cTransformBullet.velocity;
            }
        }
    }
    
    // HandlePlayerOnPlayerCollisions(entities);
    for (auto &&player1 : players)
    {
        auto &cTransformPlayer1 = player1->GetComponent<CTransform>();
        auto &cIndex1 = player1->GetComponent<CIndex>();
        for (auto &&player2 : players)
        {
            auto &cIndex2 = player2->GetComponent<CIndex>();
            if (cIndex1.playerIndex == cIndex2.playerIndex)
                continue; // can not hit self.

            auto &cTransformPlayer2 = player2->GetComponent<CTransform>();
            if (CollisionTwoCircles(cTransformPlayer1.position, PLAYER_RADIUS, cTransformPlayer2.position, PLAYER_RADIUS))
            {
                // Move the players away from each other so they barely touch.
                while(CollisionTwoCircles(cTransformPlayer1.position, PLAYER_RADIUS, cTransformPlayer2.position, PLAYER_RADIUS))
                {
                    cTransformPlayer1.position -= cTransformPlayer1.velocity / 30;
                    cTransformPlayer2.position -= cTransformPlayer2.velocity / 30;
                }

                // Calc new velocity
                Vec2f p1NewVel = ElasticCollision(cTransformPlayer1.position, cTransformPlayer2.position, cTransformPlayer1.velocity, cTransformPlayer2.velocity);
                Vec2f p2NewVel = ElasticCollision(cTransformPlayer2.position, cTransformPlayer1.position, cTransformPlayer2.velocity, cTransformPlayer1.velocity);
                cTransformPlayer1.velocity = p1NewVel;
                cTransformPlayer2.velocity = p2NewVel;
                
            }
        }
    }
}
