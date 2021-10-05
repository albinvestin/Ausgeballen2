#include "../inc/InputHandler.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
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
    int returnValue = noinput;
    // Handle single press events
    while( SDL_PollEvent( &mEvent ) != 0 )
    {
        //User requests quit
        if( mEvent.type == SDL_QUIT )
        {
            returnValue = quit;
        }
        // else if(mEvent.type == SDL_KEYDOWN)
        // {
        //     switch( mEvent.key.keysym.sym )
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
        returnValue = player1Key;
    }

    return returnValue;
}
