#include "../inc/MainMenuScene.hpp"
#include "../inc/Display.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/EntityHandler.hpp"
#include "../inc/Constants.hpp"
#include "../inc/NetworkHandler.hpp"
#include "../inc/GameScene.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#include <windows.h>
#endif
#include <inttypes.h>

void MainMenuScene::Init()
{
    //Start up SDL and create window
    if( !_display.Init() )
    {
        printf( "Failed to initialize!\n" );
        return;
    }
    //Load media
    if( !_display.LoadTextures() )
    {
        printf( "Failed to load media!\n" );
        return;
    }
}

void MainMenuScene::Start()
{
    Init();

    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        if (_inputFlags.HOST)
        {
            _networkMode = NETWORK_MODE_SERVER;
            puts("NotImplemented _inputFlags.HOST");
        }
        else if (_inputFlags.JOIN)
        {
            _networkMode = NETWORK_MODE_CLIENT;
            puts("NotImplemented _inputFlags.JOIN");
            // StartGameLoop(1);
        }
        if (_inputFlags.LOCAL_PLAY)
        {
            _networkMode = NETWORK_MODE_LOCAL;
            StartLocalPlay();
        }
        _display.RenderMainMenu();
    }

    Close();
}

void MainMenuScene::Close()
{
    _display.Close();

    //Quit SDL subsystems
    SDL_Quit();
}

void MainMenuScene::StartLocalPlay()
{
    uint8_t numberOfPlayers = 0;
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        if (_inputFlags.NUMBER2)
        {
            numberOfPlayers = 2;
        }
        else if (_inputFlags.NUMBER3)
        {
            numberOfPlayers = 3;
        }
        else if (_inputFlags.NUMBER4)
        {
            numberOfPlayers = 4;
        }
        else if (_inputFlags.NUMBER5)
        {
            numberOfPlayers = 5;
        }
        if (numberOfPlayers > 0)
        {
            // TODO instead handle the numbers of players in this loop instead of in the StartGameLoop one.
            GameScene gameScene(&_display);
            gameScene.Start(numberOfPlayers);
            return;
        }
        _display.RenderLocalPlay();
    }
}


void MainMenuScene::StartEndScore(const std::vector<std::pair<uint8_t,uint8_t>> &playerAndScoreDesc)
{
    while(!_inputFlags.QUIT && !_inputFlags.ESCAPE)
    {
        _inputHandler.EventHandler(_inputFlags);
        _display.RenderEndScore(playerAndScoreDesc);
    }
}
