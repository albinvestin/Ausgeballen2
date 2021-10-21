
#include "../inc/CollisionHandler.hpp"
#include "../inc/Vector2.hpp"
// #include "CollisionHandler.cpp"
#include "../inc/Constants.hpp"
#include <iostream>

bool FloatCompareEqual(float lhs, float rhs)
{
    if (rhs > lhs)
        return (rhs - lhs) < 0.001f;
    else
        return (lhs - rhs) < 0.001f;
}

void Test_Vector2()
{
    Vec2f p1Vel{1,0};
    Vec2f p2Vel{0,0};
    Vec2f velDiff2 = p2Vel - p1Vel;
}


void Test_ElasticCollision()
{
    // Simple horizontal one player movement
    Vec2f playerPos1{390,0}, playerPos2{400,0}, p1Vel{10,0}, p2Vel{0,0};
    Vec2f player1NewVel = ElasticCollision(playerPos1, playerPos2, p1Vel, p2Vel);
    Vec2f player2NewVel = ElasticCollision(playerPos2, playerPos1, p2Vel, p1Vel);
    // if (floatCompareEqual(player1NewVel.x) player1NewVel.x , player1NewVel.y, player2NewVel.x, player2NewVel.y )
    printf("player1NewVel: (%f,%f) , player2NewVel: (%f,%f)\n", player1NewVel.x, player1NewVel.y, player2NewVel.x, player2NewVel.y );
}

// int main( int argc, char* args[] )
// {
// 	Test_ElasticCollision();
//     // Test_Vector2();
// 	return 0;
// }