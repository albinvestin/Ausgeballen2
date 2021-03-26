#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL2/SDL.h>
#include "Texture.hpp"
#include "Vector2.hpp"

class Display
{
private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    SDL_Window* mWindow = NULL;
    SDL_Renderer* mRenderer = NULL;
    LTexture mBallTexture;
    LTexture mHearthTexture;
public:
    Display();
    ~Display();
    bool init();
    bool loadTextures();
    void close();
    void RenderAll(Vec2 playerPos);
};


#endif /* DISPLAY_HPP */
