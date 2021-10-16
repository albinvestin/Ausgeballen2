#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <vector>
// #include "Texture.hpp"
// #include "Vector2.hpp"

/* InputHandler takes care of all user input and window events.
It also keep track of which keys are assigned to which player.
*/
class InputHandler
{
private:
    SDL_Event _Event;
    //std::vector<SDL_KeyCode> _PlayerKeys; Should I use a std::_ap?
    
public:
    InputHandler();
    ~InputHandler();
    int EventHandler();
};  


// class Display
// {
// private:
//     const int SCREEN_WIDTH = 640;
//     const int SCREEN_HEIGHT = 480;
//     SDL_Window* _Window = NULL;
//     SDL_Renderer* _Renderer = NULL;
//     LTexture _BallTexture;
//     LTexture _HearthTexture;
// public:
//     Display();
//     ~Display();
//     bool init();
//     bool loadTextures();
//     void close();
//     void RenderAll(Vec2 playerPos);
// };


#endif /* INPUTHANDLER_HPP */
