#include "../inc/GameScene_MainMenu.hpp"

GameScene_MainMenu::GameScene_MainMenu(GameObj &gameObj)
    : GameScene(gameObj)
{
    std::map<SDL_Keycode, INPUT_FLAGS> mappedKeys = {
        {SDLK_q, P1SHOOT}, 
        {SDLK_p, P2SHOOT}, 
        {SDLK_v, P3SHOOT}, 
        {SDLK_z, P4SHOOT}, 
        {SDLK_m, P5SHOOT}, 
        {SDLK_h, HOST}, 
        {SDLK_j, JOIN}, 
        {SDLK_d, DISCONNECT}, 
        {SDLK_ESCAPE, ESCAPE}, 
        {SDLK_l, LOCAL_PLAY}, 
        {SDLK_2, NUMBER2}, 
        {SDLK_3, NUMBER3}, 
        {SDLK_4, NUMBER4}, 
        {SDLK_5, NUMBER5}, 
        {SDLK_5, NUMBER5}};
    _inputHandler.Init(mappedKeys);
}


bool GameScene_MainMenu::Update()
{
    _inputHandler.EventHandler(_inputFlags);
    if (_inputFlags & (INPUT_FLAGS) (1 << QUIT))
    {
        return false;
    }
    if (_inputFlags & (INPUT_FLAGS) (1 << HOST))
    {
        // _networkMode = NETWORK_MODE_SERVER;
        // StartGameLoop(2);
    }
    else if (_inputFlags & (INPUT_FLAGS) (1 << JOIN))
    {
        // _networkMode = NETWORK_MODE_CLIENT;
        // StartGameLoop(2);
    }
    if (_inputFlags & (INPUT_FLAGS) (1 << LOCAL_PLAY))
    {
        // _networkMode = NETWORK_MODE_LOCAL;
        // StartLocalPlay();
        _gameObj.ChangeScene(GAMESCENE_ID_LOCALPLAY);
    }
    // _display.RenderMainMenu();
    return true;
}
