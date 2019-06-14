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
	_set_square(cb, square, EMPTY, CHESSBOARD_MAX_COLOR);
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

bool chessboard_move(chessboard* cb, chessboard_square from, chessboard_square to)
{
    bool valid = _is_move_valid(cb, from, to);
    if (valid) _move_unchecked(cb, from, to);
    return valid;
}

void _set_square(chessboard* cb, chessboard_square square, chessboard_piecetype type, chessboard_color color)
{
    (cb->_board)[square] = (struct chessboard_piece){._type=type, ._color=color};
}

void _move_unchecked(chessboard* cb, chessboard_square from, chessboard_square to)
{
    cb->_board[to] = cb->_board[from];
    cb->_board[from] = (struct chessboard_piece){._type=EMPTY, ._color=CHESSBOARD_MAX_COLOR};
}

bool _is_move_valid(chessboard* cb, chessboard_square from, chessboard_square to)
{
    chessboard_piecetype from_type = chessboard_get_piecetype(cb, from);
    bool valid = false;
    bool from_color_valid = (cb->_board[from]._color == cb->_to_move);
    bool to_color_valid = (cb->_board[to]._color != cb->_to_move);
    if (from_color_valid && to_color_valid)
    {
	switch (from_type)
	{
	case KNIGHT:
	    valid = _is_knight_move_valid(cb, from, to);
	    break;
	case KING:
	case BISHOP:
	case ROOK:
	case QUEEN:
	default:
	    break;
	}
    }

    return valid;
}

bool _is_knight_move_valid(chessboard* cb, chessboard_square from, chessboard_square to)
{
    printf("Validating knight move from %d to %d\n");
    assert(cb->_board[from]._type == KNIGHT);
    assert(cb->_board[from]._color == cb->_to_move);
    assert(cb->_board[to]._color != cb->_to_move);
    bool valid = false;

    // Some of the problems with our board representation are
    // becoming apparent.  Checking if we have moved off the
    // board is kind of a pain.  This is ugly code, but
    // I'm not going to keep this board representation, so there's
    // no point in cleaning this up.  
    int from_rank = from / 8;
    int from_file = from % 8;
    int to_rank = to / 8;
    int to_file = to % 8;

    if ((from - to == 17) && (from_rank > 1) && (from_file > 0))
    {
	valid = true;
    }
    else if ((from - to == 10) && (from_rank > 0) && (from_file > 1))
    {
	valid = true;
    }
    else if ((from - to == 15) && (from_rank > 1) && (from_file < 7))
    {
	valid = true;
    }
    else if ((from - to == 6) && (from_rank > 0) && (from_file < 6))
    {
	valid = true;
    }
    else if ((to - from == 6) && (from_rank < 7) && (from_file < 6))
    {
	valid = true;
    }
    else if ((to - from == 17) && (from_rank < 6) && (from_file < 7))
    {
	valid = true;
    }
    else if ((to - from == 15) && (from_rank < 6) && (from_file > 0))
    {
	valid = true;
    }
    else if ((to - from == 6) && (from_rank < 7) && (from_file > 1))
    {
	valid = true;
    }

    return valid;
}
