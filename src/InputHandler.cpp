#include "../inc/InputHandler.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>

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
    while( SDL_PollEvent( &_Event ) != 0 )
    {
        //User requests quit
        if( _Event.type == SDL_QUIT )
        {
            returnValue = INPUT_QUIT;
        }
        else if(_Event.type == SDL_KEYDOWN)
        {
            if (_Event.key.keysym.sym == SDLK_b)
            {
                returnValue = INPUT_P1SHOOT;
            }
            if (_Event.key.keysym.sym == SDLK_k)
            {
                returnValue = INPUT_P2SHOOT;
            }
            if (_Event.key.keysym.sym == SDLK_h)
            {
                returnValue = INPUT_HOST;
            }
            if (_Event.key.keysym.sym == SDLK_j)
            {
                returnValue = INPUT_JOIN;
            }
            if (_Event.key.keysym.sym == SDLK_d)
            {
                returnValue = INPUT_DISCONNECT;
            }
        }
        // else if(_Event.type == SDL_KEYDOWN)
        // {
        //     switch( _Event.key.keysym.sym )
        //     {
        //         case SDLK_w:
        //         // Up
        //         returnValue = player1Key;
        //         break;
        //     }
        // }
    }

    // Handle continuous press
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    // Uses SCANCODE for key codes.
    if(keystates[SDL_SCANCODE_W])
    {
        returnValue = INPUT_P1SHOOT;
    }

    return returnValue;
}
