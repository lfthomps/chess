#ifndef MOVE_88_H
#define MOVE_88_H

#include "chess_api.h"
#include <stdint.h>

typedef struct c88_move {
    uint32_t from;
    uint32_t to;
    chess_piecetype promotion;
} c88_move;

#endif
