#include "../inc/InputHandler.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <map>
#include <algorithm>

InputHandler::InputHandler()
{}

InputHandler::~InputHandler()
{}

void InputHandler::Init(const std::map<SDL_Keycode, INPUT_FLAGS> &keyCodesToInputFlags)
{
    _keyCodesToInputFlags = keyCodesToInputFlags;
}

void InputHandler::EventHandler(INPUT_FLAGS &inputs)
{
    // Clear inputs
    INPUT_FLAGS cleared{};
    inputs = cleared;
    // Handle single press events
    while(SDL_PollEvent(&_event) != 0 )
    {
        //User requests quit
        if (_event.type == SDL_QUIT)
        {
            inputs = (1 << INPUT_FLAG_SHIFT::QUIT);
            return;
        }
        if (_event.type == SDL_KEYDOWN && _event.key.repeat == 0)
        {
            const auto& search = _keyCodesToInputFlags.find(_event.key.keysym.sym);
            if (search == _keyCodesToInputFlags.end())
            {
                continue;
            }
            inputs |= (1 << search->second);
            
            // inputs.P1SHOOT =        _event.key.keysym.sym == SDLK_q;
            // inputs.P2SHOOT =        _event.key.keysym.sym == SDLK_p;
            // inputs.P3SHOOT =        _event.key.keysym.sym == SDLK_v;
            // inputs.P4SHOOT =        _event.key.keysym.sym == SDLK_z;
            // inputs.P5SHOOT =        _event.key.keysym.sym == SDLK_m;
            // inputs.HOST =           _event.key.keysym.sym == SDLK_h;
            // inputs.JOIN =           _event.key.keysym.sym == SDLK_j;
            // inputs.DISCONNECT =     _event.key.keysym.sym == SDLK_d;
            // inputs.ESCAPE =         _event.key.keysym.sym == SDLK_ESCAPE;
            // inputs.LOCAL_PLAY =     _event.key.keysym.sym == SDLK_l;
            // inputs.NUMBER2 =        _event.key.keysym.sym == SDLK_2;
            // inputs.NUMBER3 =        _event.key.keysym.sym == SDLK_3;
            // inputs.NUMBER4 =        _event.key.keysym.sym == SDLK_4;
            // inputs.NUMBER5 =        _event.key.keysym.sym == SDLK_5;
            // inputs.NUMBER5 =        _event.key.keysym.sym == SDLK_5;
        }
    }

    // Handle continuous press
    // const uint8_t* keystates = SDL_GetKeyboardState(NULL);
    // // Uses SCANCODE for key codes.
    // if(keystates[SDL_SCANCODE_W])
    // {
    //     inputs.push_back(INPUT_P1SHOOT);
    // }
}
