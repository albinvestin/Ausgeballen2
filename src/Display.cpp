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
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

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
        return false;
    }
    
    //Create renderer for window
    _renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
    if( _renderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    //Initialize renderer color
    SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    for (uint8_t i = 0; i < ALPHABET_LENGTH; i++)
    {
        _alphabethClippingRect[i].x = ALPHABET_CHAR_WIDTH * i;
        _alphabethClippingRect[i].y = 0;
        _alphabethClippingRect[i].w = ALPHABET_CHAR_WIDTH;
        _alphabethClippingRect[i].h = ALPHABET_CHAR_HEIGHT;
    }
    
    return true;
}

bool Display::LoadFromFile(Texture* const texture, std::string relPath)
{
    if( !(*texture).LoadFromFile(relPath, _renderer) )
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
    success &= LoadFromFile(&_alphabeth, "../res/alphabet3.png");

    return success;
}

void Display::close()
{
    _ballTexture.Free();
    _bulletTexture.Free();
    _hearthTexture.Free();
    _alphabeth.Free();

    SDL_DestroyRenderer( _renderer );
    _renderer = NULL;
    SDL_DestroyWindow( _window );
    _window = NULL;

    // Quit SDL2_image system
    IMG_Quit();
}

Vec2f Display::TranslatePosToRenderSpace(const Vec2f &pos) const
{
    Vec2f renderSpacePos{pos.x + MAP_OFFSET_HORI, pos.y + MAP_OFFSET_VERT};
    return renderSpacePos;
}

void Display::RenderAll(EntityHandler const &entities) const
{
    //Clear screen
    RenderBackground();

    //Render background texture to screen
    // _HearthTexture.render( 0, 0, _Renderer );

    // Render bullets
    std::vector<Vec2f> AllBulletPos = entities.GetAllBulletPos();
    std::vector<Vec2f>::iterator it = AllBulletPos.begin();
    while (it != AllBulletPos.end())
    {
        Vec2f bulletPos = TranslatePosToRenderSpace(*it);
        _bulletTexture.Render( bulletPos.x, bulletPos.y, _renderer, NULL );
        ++it;
    }

    //Vec2 bulletPos = (*entities)..GetBullet1Pos();
    //_HearthTexture.render( bulletPos.x, bulletPos.y, _Renderer );

    //Render Player1 to the screen
    auto &allPlayers = entities.GetAllPlayers();

    for (auto &&player : allPlayers)
    {
        // Render player
        Vec2f tempPos = player.position;
        Vec2f playerPos = TranslatePosToRenderSpace(tempPos);
        switch (player.playerIndex)
        {
        case 1:
            {
                _ballTexture.ModifyColor(PLAYER_COLOR_1.r, PLAYER_COLOR_1.g, PLAYER_COLOR_1.b);
                // Render score
                std::string scoreP1 = "P1:" + std::to_string(player.score);
                RenderString(scoreP1, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT+20); // Top left
                break;
            }
        case 2:
            {
                _ballTexture.ModifyColor(PLAYER_COLOR_2.r, PLAYER_COLOR_2.g, PLAYER_COLOR_2.b);
                // Render score
                std::string scoreP2 = "P2:" + std::to_string(player.score);
                RenderString(scoreP2, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT*2+20); // Top left
                break;
            }
        case 3:
            {
                _ballTexture.ModifyColor(PLAYER_COLOR_3.r, PLAYER_COLOR_3.g, PLAYER_COLOR_3.b);
                // Render score
                std::string scoreP3 = "P3:" + std::to_string(player.score);
                RenderString(scoreP3, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT*3+20); // Top left
                break;
            }
        case 4:
            {
                _ballTexture.ModifyColor(PLAYER_COLOR_4.r, PLAYER_COLOR_4.g, PLAYER_COLOR_4.b);
                // Render score
                std::string scoreP4 = "P4:" + std::to_string(player.score);
                RenderString(scoreP4, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT*4+20); // Top left
                break;
            }
        case 5:
            {
                _ballTexture.ModifyColor(PLAYER_COLOR_5.r, PLAYER_COLOR_5.g, PLAYER_COLOR_5.b);
                // Render score
                std::string scoreP5 = "P5:" + std::to_string(player.score);
                RenderString(scoreP5, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT*5+20); // Top left
                break;
            }
        default:
            break;
        }
        _ballTexture.Render(playerPos.x, playerPos.y, _renderer, NULL);

        // Render shooting direction
        SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 150);
        float angle = player.aimDirection;
        int centerX = playerPos.x+PLAYER_RADIUS;
        int centerY = playerPos.y+PLAYER_RADIUS;
        SDL_RenderDrawLine(_renderer, centerX, centerY, centerX + cos(angle)*(2*PLAYER_RADIUS), centerY + sin(angle)*(2*PLAYER_RADIUS));
    }

    // Render map borders
    SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 255); // Black
    SDL_RenderDrawLine(_renderer, 0+MAP_OFFSET_HORI,         0+MAP_OFFSET_VERT,          MAP_WIDTH+MAP_OFFSET_HORI, 0+MAP_OFFSET_VERT); // Top
    SDL_RenderDrawLine(_renderer, 0+MAP_OFFSET_HORI,         MAP_HEIGHT+MAP_OFFSET_VERT, MAP_WIDTH+MAP_OFFSET_HORI, MAP_HEIGHT+MAP_OFFSET_VERT); // Bottom
    SDL_RenderDrawLine(_renderer, 0+MAP_OFFSET_HORI,         0+MAP_OFFSET_VERT,          0+MAP_OFFSET_HORI,         MAP_HEIGHT+MAP_OFFSET_VERT); // Left
    SDL_RenderDrawLine(_renderer, MAP_WIDTH+MAP_OFFSET_HORI, 0+MAP_OFFSET_VERT,          MAP_WIDTH+MAP_OFFSET_HORI, MAP_HEIGHT+MAP_OFFSET_VERT); // Right

    //Update screen
    SDL_RenderPresent(_renderer);
}

std::vector<uint8_t> Display::StringToAlphabetKeys(const std::string &input) const
{
    std::vector<uint8_t> result;
    for (auto &&it = input.begin(); it != input.end(); ++it)
    {
        result.push_back(ALPHABET_KEYS.at(*it));
    }
    return result;
}

void Display::RenderString(const std::string &input, int x, int y) const
{
    std::vector<uint8_t> clippingIndex = StringToAlphabetKeys(input);
    int i = 0;
    for (auto &&it = clippingIndex.begin(); it != clippingIndex.end(); ++it)
    {
        _alphabeth.Render(x + i * (ALPHABET_CHAR_WIDTH ), y, _renderer, &_alphabethClippingRect[clippingIndex[i]]);
        ++i;
    }
}

void Display::RenderBackground() const
{
    SDL_SetRenderDrawColor(_renderer, 49, 69, 88, 255);
    SDL_RenderClear(_renderer);
}

void Display::RenderMainMenu() const
{
    //Clear screen
    RenderBackground();
    // Render
    _hearthTexture.Render(SCREEN_WIDTH/2 - _hearthTexture.GetWidth()/2, SCREEN_HEIGHT/4 - _hearthTexture.GetHeight()/2, _renderer, NULL);
    std::string text{"PRESS H TO HOST"};
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT/2);
    text = "PRESS J TO JOIN";
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT/2+ALPHABET_CHAR_HEIGHT+20);
    text = "PRESS L FOR LOCAL PLAY";
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT/2+ALPHABET_CHAR_HEIGHT*2+40);
    SDL_RenderPresent(_renderer);
}

void Display::RenderLocalPlay() const
{
    //Clear screen
    RenderBackground();
    std::string text{"NO OF PLAYERS: 2 TO 5"};
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT/2);
    text = "P1:Q P2:P P3:V P4:Z P5:M";
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT-40);

    SDL_RenderPresent(_renderer);
}

void Display::RenderEndScore(const std::vector<std::pair<uint8_t,uint8_t>> &playerAndScoreDesc) const
{
    //Clear screen
    RenderBackground();
    // Render
    std::string text;
    for (size_t i=0; i < playerAndScoreDesc.size(); i++)
    {
        if (i == 0)
        {
            text = "WINNER: ";
        }
        text += "P" + std::to_string(playerAndScoreDesc[i].first) + " WITH " + std::to_string(playerAndScoreDesc[i].second) + "P";
        RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT/6+(ALPHABET_CHAR_HEIGHT+20)*i);
        text = "";
    }
    text = "PRESS ESC FOR MAIN MENU";
    RenderString(text, SCREEN_WIDTH/2 - (text.length()*ALPHABET_CHAR_WIDTH)/2, SCREEN_HEIGHT-(ALPHABET_CHAR_HEIGHT+20));
    SDL_RenderPresent(_renderer);
}
