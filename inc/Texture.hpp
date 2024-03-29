#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <string>

//Texture wrapper class
class Texture
{
private:
    //The actual hardware texture
    SDL_Texture* _texture;
    //Image dimensions
    int _width;
    int _height;
public:
    //Initializes variables
    Texture();

    //Deallocates _emory
    ~Texture();

    //Loads image at specified path
    bool LoadFromFile( std::string path, SDL_Renderer* gRenderer );

    //Deallocates texture
    void Free();

    void ModifyColor( uint8_t red, uint8_t green, uint8_t blue ) const; // TODO this is not const?!

    //Renders texture at given point
    void Render(int x, int y, SDL_Renderer* renderer, const SDL_Rect *clip) const;

    //Gets image dimensions
    int GetWidth() const;
    int GetHeight() const;
};

#endif /* TEXTURE_HPP */