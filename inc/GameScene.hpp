#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP
#include "GameObj.hpp"
#include "InputHandler.hpp"

class GameObj;

class GameScene
{
protected:
    GameObj &_gameObj;
    InputHandler _inputHandler{};
    INPUT_FLAGS _inputFlags{};
    GameScene(GameObj &gameObj) : _gameObj{gameObj} {};
public:
    ~GameScene() {};
    virtual bool Update() = 0;
};

#endif /* GAMESCENE_HPP */