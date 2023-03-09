#ifndef GAMESCENE_LOCALPLAY_HPP
#define GAMESCENE_LOCALPLAY_HPP
#include "GameScene.hpp"

class GameScene_LocalPlay : public GameScene
{
private:
    
public:
    GameScene_LocalPlay(GameObj &gameObj);
    ~GameScene_LocalPlay() {};
    bool Update();
};




#endif /* GAMESCENE_LOCALPLAY_HPP */