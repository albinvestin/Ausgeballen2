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
const uint8_t MAX_PLAYERS = 5;
const uint64_t GAME_UPDATE_TIME = 16 * SDL_GetPerformanceFrequency() / 1000;
enum INPUT_KEYS {
    INPUT_NONE,
    INPUT_QUIT,
    INPUT_P1SHOOT,
    INPUT_P2SHOOT,
    INPUT_P3SHOOT,
    INPUT_P4SHOOT,
    INPUT_P5SHOOT,
    INPUT_HOST,
    INPUT_JOIN,
    INPUT_DISCONNECT,
    INPUT_ESCAPE,
    INPUT_LOCAL_PLAY,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
};
struct INPUT_FLAGS {
    uint8_t QUIT : 1;
    uint8_t P1SHOOT : 1;
    uint8_t P2SHOOT : 1;
    uint8_t P3SHOOT : 1;
    uint8_t P4SHOOT : 1;
    uint8_t P5SHOOT : 1;
    uint8_t HOST : 1;
    uint8_t JOIN : 1;
    uint8_t DISCONNECT : 1;
    uint8_t ESCAPE : 1;
    uint8_t LOCAL_PLAY : 1;
    uint8_t NUMBER2 : 1;
    uint8_t NUMBER3 : 1;
    uint8_t NUMBER4 : 1;
    uint8_t NUMBER5 : 1;
};

enum NETWORK_TYPES {
    NETWORK_TYPE_NOTHING = 0,
    NETWORK_TYPE_FLOAT,
    NETWORK_TYPE_UINT8,
    NETWORK_TYPE_INT8,
    NETWORK_TYPE_UINT16,
    NETWORK_TYPE_INT16,
    NETWORK_TYPE_GAMESNAPSHOT,
    NETWORK_TYPE_VEC2F,
    NETWORK_TYPE_PLAYER,
    NETWORK_TYPE_BULLET,
};
enum NETWORK_ACTIONS {
    NETWORK_ACTION_SHOOT_P1,
    NETWORK_ACTION_SHOOT_P2,
    NETWORK_ACTION_SHOOT_P3,
    NETWORK_ACTION_SHOOT_P4,
    NETWORK_ACTION_SHOOT_P5,
    NETWORK_ACTION_DISCONNECT,
    NETWORK_ACTION_NEW_CONNECTION,
};
const uint8_t ALPHABET_CHAR_WIDTH = 24;
const uint8_t ALPHABET_CHAR_HEIGHT = 30;
const uint8_t ALPHABET_LENGTH = 41;
enum NETWORK_MODES {
    NETWORK_MODE_SERVER = 10,
    NETWORK_MODE_CLIENT,
    NETWORK_MODE_LOCAL
};
struct GAMELOOP_ACTIONS {
    uint16_t PlayersShooting = 0; // each bit represents one player, starting with index 0 (lowest significance) = player 1, index 1 = player 2 etc.
};
struct GAMELOOP_OUTPUT {
    uint8_t playerWon = false;
};
const struct PLAYER_COLORS {
    uint8_t r, g, b;
} PLAYER_COLOR_1{0, 0, 255}, PLAYER_COLOR_2{255, 0, 0}, PLAYER_COLOR_3{0, 255, 0}, PLAYER_COLOR_4{255, 255, 0}, PLAYER_COLOR_5{0, 255, 255};



#endif /* CONSTANTS_HPP */