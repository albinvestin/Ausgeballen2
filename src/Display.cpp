#include "../inc/Display.hpp"
#include "../inc/Texture.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#elif defined(__linux)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <math.h>

Display::Display()
{
}

Display::~Display()
{
}

bool Display::init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        _window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( _window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            _renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
            if( _renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    for (uint8_t i = 0; i < ALPHABET_LENGTH; i++)
    {
        _alphabethClippingRect[i].x = ALPHABET_CHAR_WIDTH * i;
        _alphabethClippingRect[i].y = 0;
        _alphabethClippingRect[i].w = ALPHABET_CHAR_WIDTH;
        _alphabethClippingRect[i].h = ALPHABET_CHAR_HEIGHT;
    }
    

    return success;
}

bool Display::LoadFromFile(Texture* const texture, std::string relPath)
{
    if( !(*texture).loadFromFile(relPath, _renderer) )
    {
        printf("Failed to load %s texture image!\n", relPath.c_str());
        return false;
    }
    return true;
}

bool Display::loadTextures()
{
    //Loading success flag
    bool success = true;

    success &= LoadFromFile(&_ballTexture, "../res/Image_Ball.png");
    success &= LoadFromFile(&_bulletTexture, "../res/dot.bmp");
    success &= LoadFromFile(&_hearthTexture, "../res/heart.png");
    success &= LoadFromFile(&_alphabeth, "../res/alphabet2.png");

    return success;
}

void Display::close()
{
    _ballTexture.free();
    _bulletTexture.free();
    _hearthTexture.free();
    _alphabeth.free();

    SDL_DestroyRenderer( _renderer );
    _renderer = NULL;
    SDL_DestroyWindow( _window );
    _window = NULL;

    // Quit SDL2_image system
    IMG_Quit();
}

void Display::RenderAll(EntityHandler const* entities)
{
    //Clear screen
    SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( _renderer );

    //Render background texture to screen
    // _HearthTexture.render( 0, 0, _Renderer );

    // Render bullets
    std::vector<Vec2f> AllBulletPos = (*entities).GetAllBulletPos();
    std::vector<Vec2f>::iterator it = AllBulletPos.begin();
    while (it != AllBulletPos.end())
    {
        _bulletTexture.render( (*it).x, (*it).y, _renderer, NULL );
        ++it;
    }

    //Vec2 bulletPos = (*entities)..GetBullet1Pos();
    //_HearthTexture.render( bulletPos.x, bulletPos.y, _Renderer );

    //Render Player1 to the screen
    Vec2f playerPos = (*entities).GetPlayerPos(1);
    _ballTexture.ModifyColor(255, 255, 50);
    _ballTexture.render(playerPos.x, playerPos.y, _renderer, NULL);

    // Render player1 shooting direction
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 150); // BLUE
    float angle = (*entities).GetPlayerAim(1);
    int centerX = playerPos.x+PLAYER_RADIUS;
    int centerY = playerPos.y+PLAYER_RADIUS;
    SDL_RenderDrawLine(_renderer, centerX, centerY, centerX + cos(angle)*(2*PLAYER_RADIUS), centerY + sin(angle)*(2*PLAYER_RADIUS));

    //Render Player2 to the screen
    playerPos = (*entities).GetPlayerPos(2);
    // printf("P2: (%f,%f)\n",playerPos.x, playerPos.y);
    _ballTexture.ModifyColor(255, 50, 255);
    _ballTexture.render(playerPos.x, playerPos.y, _renderer, NULL);

    // Render player2 shooting direction
    angle = (*entities).GetPlayerAim(2);
    centerX = playerPos.x+PLAYER_RADIUS;
    centerY = playerPos.y+PLAYER_RADIUS;
    SDL_RenderDrawLine(_renderer, centerX, centerY, centerX + cos(angle)*(2*PLAYER_RADIUS), centerY + sin(angle)*(2*PLAYER_RADIUS));

    //Render map borders
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // Black
    SDL_RenderDrawLine(_renderer, 0, 0, MAP_WIDTH, 0); // Top
    SDL_RenderDrawLine(_renderer, 0, MAP_HEIGHT, MAP_WIDTH, MAP_HEIGHT); // Bottom
    SDL_RenderDrawLine(_renderer, 0, 0, 0, MAP_HEIGHT); // Left
    SDL_RenderDrawLine(_renderer, MAP_WIDTH, 0, MAP_WIDTH, MAP_HEIGHT); // Right

    // _Alphabeth.render(50,50, _Renderer, NULL);
    RenderString("TEST!?", 50, 60);

    //Update screen
    SDL_RenderPresent( _renderer );
}

std::vector<uint8_t> Display::StringToAlphabetKeys(std::string input)
{
    std::vector<uint8_t> result;
    for (std::string::iterator it = input.begin(); it != input.end(); ++it)
    {
        result.push_back(ALPHABET_KEYS[*it]);
    }
    return result;
}

void Display::RenderString(std::string input, int x, int y)
{
    std::vector<uint8_t> clippingIndex = StringToAlphabetKeys(input);
    int i = 0;
    for (std::vector<uint8_t>::iterator it = clippingIndex.begin(); it != clippingIndex.end(); ++it)
    {
        _alphabeth.render(x + i * (ALPHABET_CHAR_WIDTH ), y, _renderer, &_alphabethClippingRect[clippingIndex[i]]);
        ++i;
    }
}
