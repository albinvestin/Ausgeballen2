#include "../inc/Texture.hpp"
#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#elif defined(__linux)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#endif

Texture::Texture()
{
    //Initialize
    _texture = NULL;
    _width = 0;
    _height = 0;
}

Texture::~Texture()
{
    //Deallocate
    Free();
}

bool Texture::LoadFromFile(std::string path, SDL_Renderer* gRenderer)
{
    //Get rid of preexisting texture
    Free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            _width = loadedSurface->w;
            _height = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    _texture = newTexture;
    return _texture != NULL;
}

void Texture::Free()
{
    //Free texture if it exists
    if(_texture != NULL)
    {
        SDL_DestroyTexture(_texture);
        _texture = NULL;
        _width = 0;
        _height = 0;
    }
}

void Texture::ModifyColor(uint8_t red, uint8_t green, uint8_t blue) const
{
    SDL_SetTextureColorMod(_texture, red, green, blue);
}

void Texture::Render(int x, int y, SDL_Renderer* gRenderer, const SDL_Rect *clip) const
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, _width, _height };
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, _texture, clip, &renderQuad);
}

int Texture::GetWidth() const
{
    return _width;
}

int Texture::GetHeight() const
{
    return _height;
}
