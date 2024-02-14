#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP
#include "Display.hpp"
#include "Constants.hpp"
#include "EntityHandler.hpp"
#include "NetworkHandler.hpp"
#include "InputHandler.hpp"

class MainMenuScene
{
private:
    Display _display{};
    EntityManager _entityManager{};
    InputHandler _inputHandler{};
    INPUT_FLAGS _inputFlags{};
    uint8_t _networkMode;
    void StartMainMenu();
    void StartLocalPlay();
    void StartEndScore(const std::vector<std::pair<uint8_t,uint8_t>> &playerAndScoreDesc);
    void StartGameLoop(const uint8_t numberOfplayers);
    void Init();
    void Close();

public:
    MainMenuScene() {};
    ~MainMenuScene() {};
    void Start();
};

#endif /* MAINMENUSCENE_HPP */
