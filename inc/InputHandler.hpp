#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <vector>


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

#endif /* INPUTHANDLER_HPP */
