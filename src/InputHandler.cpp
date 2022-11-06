#include "../inc/InputHandler.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <algorithm>

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

// Returns quit bool
int InputHandler::EventHandler()
{
    int returnValue = INPUT_NONE;
    // Handle single press events
    while( SDL_PollEvent( &_event ) != 0 )
    {
        //User requests quit
        if (_event.type == SDL_QUIT)
        {
            returnValue = INPUT_QUIT;
        }
        else if (_event.type == SDL_KEYDOWN)
        {
            if (_event.key.keysym.sym == SDLK_q)
            {
                returnValue = INPUT_P1SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_p)
            {
                returnValue = INPUT_P2SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_v)
            {
                returnValue = INPUT_P3SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_z)
            {
                returnValue = INPUT_P4SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_m)
            {
                returnValue = INPUT_P5SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_h)
            {
                returnValue = INPUT_HOST;
            }
            if (_event.key.keysym.sym == SDLK_j)
            {
                returnValue = INPUT_JOIN;
            }
            if (_event.key.keysym.sym == SDLK_d)
            {
                returnValue = INPUT_DISCONNECT;
            }
            if (_event.key.keysym.sym == SDLK_y)
            {
                returnValue = INPUT_SEND_P2SHOOT;
            }
            if (_event.key.keysym.sym == SDLK_ESCAPE)
            {
                returnValue = INPUT_ESCAPE;
            }
            if (_event.key.keysym.sym == SDLK_l)
            {
                returnValue = INPUT_LOCAL_PLAY;
            }
            if (_event.key.keysym.sym == SDLK_2)
            {
                returnValue = INPUT_2;
            }
            if (_event.key.keysym.sym == SDLK_3)
            {
                returnValue = INPUT_3;
            }
            if (_event.key.keysym.sym == SDLK_4)
            {
                returnValue = INPUT_4;
            }
            if (_event.key.keysym.sym == SDLK_5)
            {
                returnValue = INPUT_5;
            }
            if (_event.key.keysym.sym == SDLK_6)
            {
                returnValue = INPUT_6;
            }
        }
    }

    // Handle continuous press
    const uint8_t* keystates = SDL_GetKeyboardState(NULL);
    // Uses SCANCODE for key codes.
    if(keystates[SDL_SCANCODE_W])
    {
        returnValue = INPUT_P1SHOOT;
    }

    return returnValue;
}
