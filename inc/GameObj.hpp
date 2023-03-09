#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"
#include "Constants.hpp"
#include "EntityHandler.hpp"
#include "CollisionHandler.hpp"
#include "NetworkHandler.hpp"
#include "InputHandler.hpp"
#include "GameScene.hpp"
#include <memory.h>

class GameScene;

class GameObj
{
private:
    Display _display{};
    EntityHandler _entityHandler{};
    CollisionHandler _collisionHandler{};
    NetworkHandler _networkHandler{};
    InputHandler _inputHandler{};
    INPUT_FLAGS _inputFlags{};
    uint8_t _networkMode;
    std::unique_ptr<GameScene> _currentScene;
    bool _running = true;
    void StartMainMenu();
    void StartLocalPlay();
    void StartEndScore(const std::vector<std::pair<uint8_t,uint8_t>> &playerAndScoreDesc);
    void StartGameLoop(const uint8_t numberOfplayers);
    GAMELOOP_OUTPUT GameLoop(const GAMELOOP_ACTIONS &actions, const uint64_t currentTime, uint64_t &lastUpdateTime, uint8_t numberOfPlayers);
    void Close();

public:
    void ChangeScene(uint8_t newGameSceneID); // TODO could this be only visible to game scenes?
    GameObj();
    ~GameObj() {};
    void Start();
};

#endif /* GAMEOBJ_HPP */
