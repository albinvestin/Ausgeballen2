#include "../inc/CollisionHandler.hpp"
#include "../inc/Constants.hpp"
#include "../inc/EntityHandler.hpp"
#include "../inc/Vector2.hpp"

#include <iostream>

CollisionHandler::CollisionHandler(/* args */)
{
}

CollisionHandler::~CollisionHandler()
{
}

bool CollisionTwoCircles(Vec2f pos1, int radius1, Vec2f pos2, int radius2)
{
    Vec2f difference{pos2.x - pos1.x, pos2.y - pos1.y};
    return (Vec2Length(difference) <= radius1 + radius2);
}

void HandlePlayerOnBulletCollisions(EntityHandler &entities)
{
    // Check player-bullet collisions
    auto& AllPlayers = entities.GetAllPlayers();
    std::vector<Player>::iterator itP;
    Vec2f playerPos;
    int playerIndex;
    auto& AllBullets = entities.GetAllBullets();
    auto itB = AllBullets.begin();
    Vec2f bulletPos;
    int playerIndexOfBullet;
    
    // AllBullets = (*entities).GetAllBullets();

    itP = AllPlayers.begin();
    while (itP != AllPlayers.end())
    {
        playerPos = itP->position;
        playerIndex = itP->playerIndex;
        itB = AllBullets.begin();
        while (itB != AllBullets.end())
        {
            playerIndexOfBullet = itB->playerIndex;
            bulletPos = itB->position;
            // printf("Bullet Pos: %f, %f \n Player%uPos: %f, %f \n", bulletPos.x, bulletPos.y, playerIndex, playerPos.x, playerPos.y);
            
            if (CollisionTwoCircles(playerPos, PLAYER_RADIUS, bulletPos, BULLET_RADIUS))
            {
                if (playerIndex != playerIndexOfBullet) // Can not hit self
                {
                    printf("HIT!\n");
                    // TODO add Score to playerIndexOfBullet
                    AllPlayers.at(playerIndexOfBullet-1).score++;
                    // Add impact energy to the hit player
                    (*itP).velocity += itB->velocity;
                    // (*itP).AddVelocity((*itB).GetVelocity());
                    itB = entities.RemoveBulletFromIt(itB); // TODO We are using shared pointer so this could be updated
                    // itB = AllBullets.erase(itB);
                    continue; // Skip increase of iterator
                }
            }
            ++itB;
        }
        ++itP;
    }
}

float Squaredf(float x)
{
    return x*x;
}

Vec2f ElasticCollision(Vec2f playerPos1, Vec2f playerPos2, Vec2f p1Vel, Vec2f p2Vel)
{
    Vec2f velDiff1 = p1Vel - p2Vel;
    Vec2f posDiff1 = playerPos1 - playerPos2;
    float dotProd1 = DotProduct(velDiff1, posDiff1);
    float squaredNorm1 = Squaredf(Vec2Length(posDiff1));
    Vec2f nominator1 = dotProd1 * posDiff1;

    Vec2f p1NewVel = p1Vel - (nominator1 / squaredNorm1);

    return p1NewVel;
}

void HandlePlayerOnPlayerCollisions(EntityHandler &entities)
{
    // printf("Collision adress: %d\n", &(*(*entities).GetAllPlayers())[0] );
    // Check player-bullet collisions
    auto& AllPlayers = entities.GetAllPlayers();
    std::vector<Player>::iterator itP1;
    std::vector<Player>::iterator itP2;
    Vec2f playerPos1, playerPos2;

    Vec2f bulletPos;
    
    // printf("AllPlayers adress: %d\n", AllPlayers);


    itP1 = AllPlayers.begin();
    while (itP1 != AllPlayers.end())
    {
        playerPos1 = (*itP1).position;
        // playerIndex1 = (*itP1).GetPlayerIndex();
        itP2 = AllPlayers.begin();
        while (itP2 != AllPlayers.end())
        {
            // printf("Iterator adress: %d\n", itP1);
            playerPos2 = (*itP2).position;
            // playerIndex2 = (*itP2).GetPlayerIndex();
            if (CollisionTwoCircles(playerPos1, PLAYER_RADIUS, playerPos2, PLAYER_RADIUS))
            {
                if (itP1 != itP2) // Can not hit self
                {
                    // printf("Player bump!\n");
                    Vec2f p1Vel = (*itP1).velocity;
                    Vec2f p2Vel = (*itP2).velocity;

                    Vec2f p1NewVel = ElasticCollision(playerPos1, playerPos2, p1Vel, p2Vel);
                    Vec2f p2NewVel = ElasticCollision(playerPos2, playerPos1, p2Vel, p1Vel);
                    
                    // (*itP1).SetVelocity(p1NewVel);
                    (*itP1).velocity = p1NewVel;
                    // (*itP2).SetVelocity(p2NewVel);
                    (*itP2).velocity = p2NewVel;
                    return; // TODO handle this for more players, should mark the current two players as handled and continue with unhandled ones.

                }
            }
            ++itP2;
        }
        ++itP1;
    }
}

void CollisionHandler::HandleCollisons(EntityHandler & entities)
{
    HandlePlayerOnBulletCollisions(entities);
    HandlePlayerOnPlayerCollisions(entities);

}

