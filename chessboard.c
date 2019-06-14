#include <stdint.h>
#include <stdlib.h>
#include "chessboard_api.h"
#include "chessboard.h"
#include <assert.h>

#include <stdio.h> // Just for debugging

chessboard* chessboard_allocate()
{
    chessboard* cb = (chessboard *)malloc(sizeof(chessboard));
    if (cb)
    {
	cb->_board = (struct chessboard_piece *)malloc(sizeof(struct chessboard_piece) * 64);
	if (cb->_board)
	{
	    cb->_to_move = WHITE;
	}
	else
	{
	    free(cb);
	    printf("DEBUG: Failed to allocate cb._board\n");
	    return 0;
	}
    }
    else
    {
	printf("DEBUG: Failed to allocate cb\n");
    }
    return cb;    
}

void chessboard_free(chessboard* cb)
{
    assert(cb && "Tried to free null pointer to chessboard");
    assert(cb->_board && "Tried to free chessboard with null _board");

    free(cb->_board);
    free(cb);
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

chessboard_piecetype chessboard_get_piecetype(chessboard* cb, chessboard_square square)
{
    return cb->_board[square]._type;
}

chessboard_color chessboard_get_color(chessboard* cb, chessboard_square square)
{
    return cb->_board[square]._color;
}

void _set_square(chessboard* cb, chessboard_square square, chessboard_piecetype type, chessboard_color color)
{
    (cb->_board)[square] = (struct chessboard_piece){._type=type, ._color=color};
}
