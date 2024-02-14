#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP
#include "Display.hpp"
#include "Constants.hpp"
#include "EntityHandler.hpp"
#include "NetworkHandler.hpp"
#include "InputHandler.hpp"

class GameScene
{
private:
    Display *_display{};
    EntityManager _entityManager{};
    NetworkHandler _networkHandler{};
    InputHandler _inputHandler{};
    INPUT_FLAGS _inputFlags{};
    uint8_t _networkMode;

    void Close();
    void SMovement();
    void SPlayerActions();
    void SpawnBullet(float aimDirection, const Vec2f &position, uint8_t playerIndex);
    void SUserInput();
    void SCollisions();
    void SCheckEndScore();
public:
    GameScene(Display *display) : _display{display} {};
    ~GameScene() {};
    void Start(uint8_t numberOfPlayers);
};


#endif /* #ifndef GAMESCENE_HPP */ 