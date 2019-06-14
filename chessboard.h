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

void _set_square(chessboard* cb, chessboard_square square, chessboard_piecetype type, chessboard_color color);
void _move_unchecked(chessboard* cb, chessboard_square from, chessboard_square to);

#endif
