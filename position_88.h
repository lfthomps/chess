#ifndef POSITION_88_H
#define POSITION_88_H

#include "chess_api.h"
#include "board_88.h"
#include <stdint.h>

typedef struct c88_position {
    c88_board board;
    chess_color to_move;
    uint32_t ep_square;
    uint8_t castle_rights;
    uint8_t move_count;
    uint8_t reversible_moves;    
} c88_position;

#endif
