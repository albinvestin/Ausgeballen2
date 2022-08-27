#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <stdint.h>
#include <SDL2/SDL.h>
const uint16_t SCREEN_WIDTH = 1100;
const uint16_t SCREEN_HEIGHT = 750;
const uint16_t MAP_WIDTH = 700;
const uint16_t MAP_HEIGHT = 700;
const uint16_t MAP_OFFSET_HORI = (SCREEN_WIDTH-MAP_WIDTH)/2;
const uint16_t MAP_OFFSET_VERT = 25;
const uint16_t BULLET_INIT_VEL = 20;
const float PLAYER_FRICTION = 0.99;
const float PLAYER_RECOIL = 10;
const uint16_t PLAYER_RADIUS = 15;
const uint16_t BULLET_RADIUS = 10;
const uint8_t END_SCORE = 2;
const uint8_t MAX_PLAYERS = 2;
const Uint64 GAME_UPDATE_TIME = 16 * SDL_GetPerformanceFrequency() / 1000;
enum INPUT_KEYS {
    INPUT_NONE,
    INPUT_QUIT,
    INPUT_P1SHOOT,
    INPUT_P2SHOOT,
    INPUT_SEND_P1SHOOT,
    INPUT_SEND_P2SHOOT,
    INPUT_HOST,
    INPUT_JOIN,
    INPUT_DISCONNECT,
    INPUT_ESCAPE
};
enum NETWORK_TYPES {
    NETWORK_TYPE_NOTHING = 0,
    NETWORK_TYPE_FLOAT,
    NETWORK_TYPE_UINT8,
    NETWORK_TYPE_INT8,
    NETWORK_TYPE_UINT16,
    NETWORK_TYPE_INT16,
    NETWORK_TYPE_GAMESNAPSHOT,
    NETWORK_TYPE_VEC2F
};
enum NETWORK_ACTIONS {
    NETWORK_ACTION_SHOOT_P1 = 100,
    NETWORK_ACTION_SHOOT_P2
};
const uint8_t ALPHABET_CHAR_WIDTH = 24;
const uint8_t ALPHABET_CHAR_HEIGHT = 30;
const uint8_t ALPHABET_LENGTH = 41;

#endif /* CONSTANTS_HPP */