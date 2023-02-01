#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"
#include "Constants.hpp"
#include "EntityHandler.hpp"
#include "CollisionHandler.hpp"

class GameObj
{
private:
    Display _display{};
    EntityHandler _entityHandler{};
    CollisionHandler _collisionHandler{};
    void StartMainMenu();
    int StartLocalPlay();
    int StartEndScore(std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc);
    int StartGameLoop(int input);
    GAMELOOP_OUTPUT GameLoop(GAMELOOP_ACTIONS actions, uint64_t currentTime, uint64_t lastUpdateTime);
    void Close();
public:
    GameObj() {};
    ~GameObj() {};
    void Start();
};

#endif /* GAMEOBJ_HPP */
