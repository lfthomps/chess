#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdbool.h>
#include "chessboard_api.h"

struct chessboard_piece {
    enum chessboard_piecetype _type;
    enum chessboard_color _color;
};

struct chessboard {
    enum chessboard_color _to_move;
    struct chessboard_piece* _board;
};

void _set_square(chessboard* cb, enum chessboard_square square, enum chessboard_piecetype type, enum chessboard_color color);

#endif
