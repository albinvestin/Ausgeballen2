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
#include <map>

class Display
{
private:
    SDL_Window* _Window = NULL;
    SDL_Renderer* _Renderer = NULL;
    LTexture _BallTexture;
    LTexture _HearthTexture;
    LTexture _BulletTexture;
    LTexture _Alphabeth;
    SDL_Rect _AlphabethClippingRect[ALPHABET_LENGTH];
    std::vector<unsigned char> StringToAlphabetKeys(std::string input);
    void RenderString(std::string input, int x, int y);
    std::map<char, unsigned char> ALPHABET_KEYS 
    {{'A', 0},
    {'B', 1},
    {'C', 2},
    {'D', 3},
    {'E', 4},
    {'F', 5},
    {'G', 6},
    {'H', 7},
    {'I', 8},
    {'J', 9},
    {'K', 10},
    {'L', 11},
    {'M', 12},
    {'N', 13},
    {'O', 14},
    {'P', 15},
    {'Q', 16},
    {'R', 17},
    {'S', 18},
    {'T', 19},
    {'U', 20},
    {'V', 21},
    {'W', 22},
    {'X', 23},
    {'Y', 24},
    {'Z', 25}};
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
