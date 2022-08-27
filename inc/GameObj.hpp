#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"

class GameObj
{
private:
    Display _display{};
    void StartMainMenu();
    int StartEndScore(std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc);
    int StartGameLoop(int input);
    void Close();
public:
    GameObj() {};
    ~GameObj() {};
    void Start();
};

#endif /* GAMEOBJ_HPP */
