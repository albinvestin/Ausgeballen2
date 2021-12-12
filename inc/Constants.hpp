#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <stdint.h>
const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 480;
const uint16_t MAP_OFFSET_HORI = 20;
const uint16_t MAP_OFFSET_VERT = 20;
const uint16_t MAP_WIDTH = 600;
const uint16_t MAP_HEIGHT = 400;
const uint16_t BULLET_INIT_VEL = 20;
const float PLAYER_FRICTION = 0.99;
const float PLAYER_RECOIL = 10;
const uint16_t PLAYER_RADIUS = 15;
const uint16_t BULLET_RADIUS = 10;
enum INPUTKEY {INPUT_NONE = 0, INPUT_QUIT, INPUT_P1SHOOT, INPUT_P2SHOOT, INPUT_HOST, INPUT_JOIN, INPUT_DISCONNECT};
enum NETWORKTYPES {
    NETWORK_TYPE_FLOAT,
    NETWORK_TYPE_UINT8,
    NETWORK_TYPE_INT8,
    NETWORK_TYPE_UINT16,
    NETWORK_TYPE_INT16
};
const uint8_t ALPHABET_CHAR_WIDTH = 24;
const uint8_t ALPHABET_CHAR_HEIGHT = 30;
const uint8_t ALPHABET_LENGTH = 40;

#endif /* CONSTANTS_HPP */