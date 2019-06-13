#include <stdint.h>
#include "chessboard.h"

void _set_square(chessboard* cb, enum chessboard_square square, enum chessboard_piecetype type, enum chessboard_color color)
{
    (cb->_board)[square] = (struct chessboard_piece){._type=type, ._color=color};
}

void chessboard_initialize_board(chessboard* cb)
{
    for (enum chessboard_square square = A6; square < A2; square++)
    {
	_set_square(cb, square, EMPTY, WHITE);
    }
    for (enum chessboard_square square = A7; square < A6; square++)
    {
	_set_square(cb, square, PAWN, BLACK);
    }
    for (enum chessboard_square square = A2; square < A1; square++)
    {
	_set_square(cb, square, PAWN, WHITE);
    }
    _set_square(cb, A8, ROOK, BLACK);
    _set_square(cb, B8, KNIGHT, BLACK);
    _set_square(cb, C8, BISHOP, BLACK);
    _set_square(cb, D8, QUEEN, BLACK);
    _set_square(cb, E8, KING, BLACK);
    _set_square(cb, F8, BISHOP, BLACK);
    _set_square(cb, G8, KNIGHT, BLACK);
    _set_square(cb, H8, ROOK, BLACK);
    _set_square(cb, A1, ROOK, WHITE);
    _set_square(cb, B1, KNIGHT, WHITE);
    _set_square(cb, C1, BISHOP, WHITE);
    _set_square(cb, D1, QUEEN, WHITE);
    _set_square(cb, E1, KING, WHITE);
    _set_square(cb, F1, BISHOP, WHITE);
    _set_square(cb, G1, KNIGHT, WHITE);
    _set_square(cb, H1, ROOK, WHITE);
}

enum chessboard_piecetype chessboard_get_piecetype(chessboard* cb, enum chessboard_square square)
{
    return cb->_board[square]._type;
}

enum chessboard_color chessboard_get_color(chessboard* cb, enum chessboard_square square)
{
    return cb->_board[square]._color;
}
