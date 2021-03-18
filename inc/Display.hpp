#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL2/SDL.h>
#include "Texture.hpp"

class Display
{
private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    LTexture gFooTexture;
    LTexture gBackgroundTexture;
public:
    Display();
    ~Display();
    bool init();
    bool loadTextures();
    void close();
    void RenderAll();
};


#endif /* DISPLAY_HPP */
