#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <string>

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* gRenderer );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Renderer* gRenderer );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};


#endif /* TEXTURE_HPP */