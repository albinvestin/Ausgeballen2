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

bool CollisionHandler::CollisionTwoCircles(Vec2f pos1, int radius1, Vec2f pos2, int radius2)
{
    Vec2f difference{pos2.x - pos1.x, pos2.y - pos1.y};
    return (Vec2Length(difference) <= radius1 + radius2);
}

bool CollisionHandler::CheckCollisons(EntityHandler *entities)
{
    // Check player-bullet collisions
    std::vector<Player> *AllPlayers;
    std::vector<Player>::iterator itP;
    Vec2f playerPos;
    int playerIndex;
    std::vector<Bullet> *AllBullets;
    std::vector<Bullet>::iterator itB;
    Vec2f bulletPos;

    int playerIndexOfBullet;

    AllPlayers = (*entities).GetAllPlayers();
    AllBullets = (*entities).GetAllBullets();
    itP = (*AllPlayers).begin();
    while (itP != (*AllPlayers).end())
    {
        playerPos = (*itP).GetPos();
        playerIndex = (*itP).GetPlayerIndex();
        itB = (*AllBullets).begin();
        while (itB != (*AllBullets).end())
        {
            playerIndexOfBullet = (*itB).GetPlayerIndex();
            bulletPos = (*itB).GetPos();
            
            if (CollisionTwoCircles(playerPos, PLAYER_RADIUS, bulletPos, BULLET_RADIUS))
            {
                if (playerIndex != playerIndexOfBullet)
                {
                    printf("HIT!\n");
                    // TODO add Score to playerIndexOfBullet
                    // Add impact energy to the hit player
                    (*itP).AddVelocity((*itB).GetVelocity());
                    itB = (*AllBullets).erase(itB);
                    continue; // Skip increase of iterator
                }
            }

            ++itB;
        }

        ++itP;
    }

    


}

