#include "../inc/GameScene_LocalPlay.hpp"

GameScene_LocalPlay::GameScene_LocalPlay(GameObj &gameObj)
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


bool GameScene_LocalPlay::Update()
{
    _inputHandler.EventHandler(_inputFlags);
    if (_inputFlags & (INPUT_FLAGS) (1 << QUIT))
    {
        return false;
    }
    int numberOfPlayers = 0;
    if (_inputFlags & (INPUT_FLAGS) (1 << NUMBER2))
    {
         numberOfPlayers = 2;
    }
    else if (_inputFlags & (INPUT_FLAGS) (1 << NUMBER3))
    {
        numberOfPlayers = 3;
    }
    else if (_inputFlags & (INPUT_FLAGS) (1 << NUMBER4))
    {
        numberOfPlayers = 4;
    }
    else if (_inputFlags & (INPUT_FLAGS) (1 << NUMBER5))
    {
        numberOfPlayers = 5;
    }
    if (numberOfPlayers > 0)
    {
        // TODO instead handle the numbers of players in this loop instead of in the StartGameLoop one.
        // StartGameLoop(numberOfPlayers);
        return;
    }


        // int numberOfPlayers = 0;
    // while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    // {
    //     _inputHandler.EventHandler(_inputFlags);
    //     if (_inputFlags.NUMBER2)
    //     {
    //         numberOfPlayers = 2;
    //     }
    //     else if (_inputFlags.NUMBER3)
    //     {
    //         numberOfPlayers = 3;
    //     }
    //     else if (_inputFlags.NUMBER4)
    //     {
    //         numberOfPlayers = 4;
    //     }
    //     else if (_inputFlags.NUMBER5)
    //     {
    //         numberOfPlayers = 5;
    //     }
    //     if (numberOfPlayers > 0)
    //     {
    //         // TODO instead handle the numbers of players in this loop instead of in the StartGameLoop one.
    //         StartGameLoop(numberOfPlayers);
    //         return;
    //     }
    //     _display.RenderLocalPlay();
    // }
    return true;
}
