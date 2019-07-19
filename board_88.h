#ifndef BOARD_88_H
#define BOARD_88_H

#include "chess_api.h"
#include <stdint.h>

#define C88_MAX_INDEX 128
#define C88_MAX_PIECES 16

typedef struct c88_piece {
    chess_color color;
    chess_piecetype type;
    uint32_t square;
} c88_piece;

typedef struct c88_board {
    c88_piece* board[C88_MAX_INDEX];
    c88_piece piecelist[2][C88_MAX_PIECES];
} c88_board;

#endif
