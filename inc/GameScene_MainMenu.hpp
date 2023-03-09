#ifndef GAMESCENE_MAINMENU_HPP
#define GAMESCENE_MAINMENU_HPP
#include "GameScene.hpp"

class GameScene_MainMenu : public GameScene
{
private:
    
public:
    GameScene_MainMenu(GameObj &gameObj);
    ~GameScene_MainMenu() {};
    bool Update();
};




#endif /* GAMESCENE_MAINMENU_HPP */