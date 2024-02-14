#include "../inc/GameScene.hpp"
#include <algorithm>
#include <cmath>

void GameScene::SMovement() // TODO move this to separate class?
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

void GameScene::SpawnBullet(float aimDirection, const Vec2f &position, uint8_t playerIndex)
{
    auto newBullet = _entityManager.AddEntity(ENTITY_TAG_BULLET);
    newBullet->AddComponent<CTransform>(position, Vec2f((float)(BULLET_INIT_VEL * cos(aimDirection)),
                                                       (float)(BULLET_INIT_VEL * sin(aimDirection))));
    newBullet->AddComponent<CIndex>(playerIndex);
}

void GameScene::SPlayerActions()
{
    auto &players = _entityManager.GetEntities(ENTITY_TAG_PLAYER);
    for (auto &&player : players)
    {
        auto &cCannon = player->GetComponent<CCannon>();
        if (cCannon.isShooting)
        {
            puts("Got Shot");
            auto &cTransform = player->GetComponent<CTransform>();
            auto &cIndex = player->GetComponent<CIndex>();
            cCannon.isShooting = false;
            SpawnBullet(cCannon.aimDirection, cTransform.position, cIndex.playerIndex);
            cTransform.velocity.x -= (cos(cCannon.aimDirection) * PLAYER_RECOIL);
            cTransform.velocity.y -= (sin(cCannon.aimDirection) * PLAYER_RECOIL);
        }
    }
}

void GameScene::SUserInput()
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

void GameScene::SCollisions()
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

bool SortBySec(const std::pair<uint8_t,uint8_t> &a, const std::pair<uint8_t,uint8_t> &b)
{
    return (a.second > b.second);
}

void GameScene::SCheckEndScore()
{
    auto &players = _entityManager.GetEntities(ENTITY_TAG_PLAYER);
    for (auto &&player : players)
    {
        if (player->GetComponent<CScore>().score >= END_SCORE)
        {
            std::vector<std::pair<uint8_t,uint8_t>> playerAndScoreDesc;
            for (auto &&pScoreSummary : players)
            {
                auto playerScore = pScoreSummary->GetComponent<CScore>().score;
                auto playerIndex = pScoreSummary->GetComponent<CIndex>().playerIndex;
                playerAndScoreDesc.push_back(std::make_pair(playerIndex, playerScore));
            }
            std::sort(playerAndScoreDesc.begin(), playerAndScoreDesc.end(), SortBySec);
            // TODO StartEndScore(playerAndScoreDesc);
            return;
        }
    }
}
