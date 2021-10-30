#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "Texture.hpp"
#include "Vector2.hpp"
#include "EntityHandler.hpp"

class Display
{
private:
    SDL_Window* _Window = NULL;
    SDL_Renderer* _Renderer = NULL;
    LTexture _BallTexture;
    LTexture _HearthTexture;
    LTexture _BulletTexture;
public:
    Display();
    ~Display();
    bool init();
    bool LoadFromFile(LTexture *const texture, std::string relPath); // Help function for loadTextures
    bool loadTextures();
    void close();
    void RenderAll(EntityHandler const*);
};


#endif /* DISPLAY_HPP */
