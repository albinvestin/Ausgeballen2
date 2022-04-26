#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <string>

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates _emory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path, SDL_Renderer* gRenderer );

        //Deallocates texture
        void free();

        void ModifyColor( uint8_t red, uint8_t green, uint8_t blue );

        //Renders texture at given point
        void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip);

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* _Texture;

        //Image dimensions
        int _Width;
        int _Height;
};


#endif /* TEXTURE_HPP */