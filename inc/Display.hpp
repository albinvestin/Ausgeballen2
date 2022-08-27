#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Texture.hpp"
#include "Vector2.hpp"
#include "EntityHandler.hpp"
#include <map>

class Display
{
private:
    SDL_Window* _window = NULL;
    SDL_Renderer* _renderer = NULL;
    Texture _ballTexture;
    Texture _hearthTexture;
    Texture _bulletTexture;
    Texture _alphabeth;
    SDL_Rect _alphabethClippingRect[ALPHABET_LENGTH];
    std::vector<uint8_t> StringToAlphabetKeys(std::string input);
    void RenderString(std::string input, int x, int y);
    Vec2f TranslatePosToRenderSpace(const Vec2f pos);
    const std::map<char, uint8_t> ALPHABET_KEYS 
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
    {'Z', 25},
    {'0', 26},
    {'1', 27},
    {'2', 28},
    {'3', 29},
    {'4', 30},
    {'5', 31},
    {'6', 32},
    {'7', 33},
    {'8', 34},
    {'9', 35},
    {'.', 36},
    {':', 37},
    {'!', 38},
    {'?', 39},
    {' ', 40}};
public:
    Display();
    ~Display();
    bool init();
    bool LoadFromFile(Texture *const texture, std::string relPath); // Help function for loadTextures
    bool loadTextures();
    void close();
    void RenderAll(EntityHandler const*);
    void RenderMainMenu();
    void RenderLocalPlay();
    void RenderEndScore(std::vector< std::pair<uint8_t,uint8_t> > playerAndScoreDesc);
};


#endif /* DISPLAY_HPP */
