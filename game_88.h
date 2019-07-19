#ifndef GAME_88_H
#define GAME_88_H

#include "chess_api.h"
#include "board_88.h"
#include "move_88.h"
#include <stdint.h>

typedef struct c88_game {
    c88_position* pos_tree;
    uint32_t* freelist;
    c88_move* movelist;
} c88_game;

#endif
