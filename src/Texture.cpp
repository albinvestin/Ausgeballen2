#include "../inc/Texture.hpp"
#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#elif defined(__linux)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL_image.h>
#include <SDL.h>
#endif

LTexture::LTexture()
{
	//Initialize
	_Texture = NULL;
	_Width = 0;
	_Height = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path, SDL_Renderer* gRenderer )
{
	//Get rid of preexisting texture
	free();

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
			_Width = loadedSurface->w;
			_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	_Texture = newTexture;
	return _Texture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( _Texture != NULL )
	{
		SDL_DestroyTexture( _Texture );
		_Texture = NULL;
		_Width = 0;
		_Height = 0;
	}
}

void LTexture::render( int x, int y, SDL_Renderer* gRenderer )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, _Width, _Height };
	SDL_RenderCopy( gRenderer, _Texture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
	return _Width;
}

int LTexture::getHeight()
{
	return _Height;
}
