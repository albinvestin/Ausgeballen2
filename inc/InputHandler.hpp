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
    SDL_Event mEvent;
    //std::vector<SDL_KeyCode> mPlayerKeys; Should I use a std::map?
    enum returnValues {noinput = 0, quit = 1, player1Key}; // TODO: use better camelcase?
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
//     SDL_Window* mWindow = NULL;
//     SDL_Renderer* mRenderer = NULL;
//     LTexture mBallTexture;
//     LTexture mHearthTexture;
// public:
//     Display();
//     ~Display();
//     bool init();
//     bool loadTextures();
//     void close();
//     void RenderAll(Vec2 playerPos);
// };


#endif /* INPUTHANDLER_HPP */
