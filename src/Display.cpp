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

bool Display::Init()
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

bool Display::LoadTextures()
{
    //Loading success flag
    bool success = true;

    success &= LoadFromFile(&_ballTexture, "../res/Image_Ball.png");
    success &= LoadFromFile(&_bulletTexture, "../res/dot.bmp");
    success &= LoadFromFile(&_hearthTexture, "../res/heart.png");
    success &= LoadFromFile(&_alphabeth, "../res/alphabet3.png");

    return success;
}

void Display::Close()
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

void Display::RenderAll(EntityManager const &entityManager) const
{
    //Clear screen
    RenderBackground();

    //Render background texture to screen
    // _HearthTexture.render( 0, 0, _Renderer );

    // Render entities
    auto &bullets = entityManager.GetEntities(ENTITY_TAG_BULLET);
    for (auto &&bullet : bullets)
    {
        auto &cTransform = bullet->GetComponent<CTransform>();
        Vec2f pos{TranslatePosToRenderSpace(cTransform.position)};
        _bulletTexture.Render(pos.x, pos.y, _renderer, NULL); // TODO add texture as component.
    }

    auto &players = entityManager.GetEntities(ENTITY_TAG_PLAYER);
    for (auto &&player : players)
    {
        auto &cIndex = player->GetComponent<CIndex>();
        auto &cTransform = player->GetComponent<CTransform>();
        auto &cCannon = player->GetComponent<CCannon>();
        auto &cScore = player->GetComponent<CScore>();

        // Render Player
        Vec2f pos{TranslatePosToRenderSpace(cTransform.position)};
        _ballTexture.ModifyColor(PLAYER_COLORS[cIndex.playerIndex][0], PLAYER_COLORS[cIndex.playerIndex][1], PLAYER_COLORS[cIndex.playerIndex][2]);
        _ballTexture.Render(pos.x, pos.y, _renderer, NULL); // TODO add texture as component.
    
        // Render shooting direction
        SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 150);
        float angle = cCannon.aimDirection;
        int centerX = pos.x+PLAYER_RADIUS;
        int centerY = pos.y+PLAYER_RADIUS;
        SDL_RenderDrawLine(_renderer, centerX, centerY, centerX + cos(angle)*(2*PLAYER_RADIUS), centerY + sin(angle)*(2*PLAYER_RADIUS));

        // Render score
        std::string score = "P" + std::to_string(cIndex.playerIndex) + ":" + std::to_string(cScore.score);
        RenderString(score, 10, MAP_OFFSET_VERT + ALPHABET_CHAR_HEIGHT*cIndex.playerIndex+20);
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
