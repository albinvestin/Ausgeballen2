#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Constants.hpp"
#include <map>


/* InputHandler takes care of all user input and window events.
It also keep track of which keys are assigned to which player.
*/
class InputHandler
{
private:
    SDL_Event _event;
    std::map<SDL_Keycode , INPUT_FLAGS> _keyCodesToInputFlags;
    //std::vector<SDL_KeyCode> _PlayerKeys; Should I use a std::map?
    
public:
    InputHandler();
    ~InputHandler();
    void Init(const std::map<SDL_Keycode, INPUT_FLAGS> &keyCodesToInputFlags);
    void EventHandler(INPUT_FLAGS &inputs);
};  

#endif /* INPUTHANDLER_HPP */
