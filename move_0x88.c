#include "move_0x88.h"
#include <assert.h>
#include <stdint.h>

#include <stdio.h> //For debugging

bool chessboard_move(chessboard* cb, chessboard_square from, chessboard_square to)
{
    uint32_t from_index = _get_internal_square(from);
    uint32_t to_index = _get_internal_square(to);
    bool valid = _is_move_valid(cb, from_index, to_index);
    if (valid) _move_unchecked(cb, from_index, to_index);
    DEBUG_validate_board(cb);
    return valid;
}

void _move_unchecked(chessboard* cb, uint32_t from, uint32_t to)
{
    _clear_square(cb, to);
    cb->board[to] = cb->board[from];
    cb->board[to]->square = to;
    cb->board[from] = NULL;
}

bool _is_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));
    
    bool valid = false;
    bool from_color_valid = (_chessboard_get_color(cb, from) == cb->to_move);
    bool to_color_valid = (_chessboard_get_color(cb, to) != cb->to_move);
    if (from_color_valid && to_color_valid)
    {
	switch (_chessboard_get_piecetype(cb, from))
	{
	case KNIGHT:
	    valid = _is_knight_move_valid(cb, from, to);
	    break;
	case KING:
	    valid = _is_king_move_valid(cb, from, to);
	    break;
	case BISHOP:
	    valid = _is_bishop_move_valid(cb, from, to);
	    break;
	case ROOK:
	    valid = _is_rook_move_valid(cb, from, to);
	    break;
	case QUEEN:
	    valid = _is_bishop_move_valid(cb, from, to) || _is_rook_move_valid(cb, from, to);
	    break;
	case PAWN:
	    valid = _is_pawn_move_valid(cb, from, to);
	    break;
	default:
	    break;
	}
    }

    return valid;
}

bool _is_knight_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    assert(_chessboard_get_piecetype(cb, from) == KNIGHT);
    assert(_chessboard_get_color(cb, from) == cb->to_move);
    assert(_chessboard_get_color(cb, to) != cb->to_move);
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));

    bool valid = false;
    int32_t diff = to - from;
    switch (diff)
    {
    case 33:
    case 31:
    case 18:
    case 14:
    case -33:
    case -31:
    case -18:
    case -14:
	valid = true;
	break;
    default:
	break;
    }
    return valid;
}

bool _is_king_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    assert(_chessboard_get_piecetype(cb, from) == KING);
    assert(_chessboard_get_color(cb, from) == cb->to_move);
    assert(_chessboard_get_color(cb, to) != cb->to_move);
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));

    bool valid = false;
    int32_t diff = to - from;
    switch (diff)
    {
    case 1:
    case 17:
    case 16:
    case 15:
    case -1:
    case -17:
    case -16:
    case -15:
	valid = true;
	break;
    default:
	break;
    }
    return valid;
}

bool _is_bishop_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    assert(_chessboard_get_piecetype(cb, from) == BISHOP || _chessboard_get_piecetype(cb, from) == QUEEN);
    assert(_chessboard_get_color(cb, from) == cb->to_move);
    assert(_chessboard_get_color(cb, to) != cb->to_move);
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));

    bool valid = false;
    int32_t diff = to - from;
    int32_t bishop_directions[2] = {17, 15};
    for (int i = 0; i < 2; i++)
    {
	int32_t direction = (to > from) ? bishop_directions[i] : -bishop_directions[i];
	if (diff % direction == 0)
	{
	    while (from != to)
	    {
		from += direction;
		if (_chessboard_get_piecetype(cb, from) != EMPTY) break;
	    }
	    if (from == to) valid = true;
	}
    }

    return valid;
}

bool _is_rook_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    printf("Validating rook move from %d to %d\n", from, to);
    assert(_chessboard_get_piecetype(cb, from) == ROOK || _chessboard_get_piecetype(cb, from) == QUEEN);
    assert(_chessboard_get_color(cb, from) == cb->to_move);
    assert(_chessboard_get_color(cb, to) != cb->to_move);
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));

    bool valid = false;
    int32_t diff = to - from;
    printf("Diff = %d\n", diff);
    // It would be nice to treat this exactly like the bishop moves, but
    // with directions 16 and 1 instead of 15 and 17.  This works fine
    // for 16 (relying on the fact that, for 0x88 boards, as long as the
    // to and from squares are legal, moves that change rank can't wrap
    // around the board).  However, the diff for any move is divisible
    // by 1, and the method from _is_bishop_move_valid would allow
    // many moves that wrap around the board.  We therefore have to
    // treat horizontal rook moves specially.  We solve this by checking
    // if to and from are on the same rank.  
    if (diff % 16 == 0)
    {
	int32_t direction = (to > from) ? 16 : -16;
	if (diff % direction == 0)
	{
	    printf("Testing direction %d\n", direction);
	    while (from != to)
	    {
		from += direction;
		if (_chessboard_get_piecetype(cb, from) != EMPTY) break;
	    }
	    if (from == to) valid = true;
	}
    }
    else if ((to / 16) == (from / 16))
    {
	int32_t direction = (to > from) ? 1 : -1;
	printf("Testing direction %d\n", direction);
	while (from != to)
	{
	    from += direction;
	    if (_chessboard_get_piecetype(cb, from) != EMPTY) break;
	}
	if (from == to) valid = true;
    }

    return valid;
}

bool _is_pawn_move_valid(chessboard* cb, uint32_t from, uint32_t to)
{
    assert(_chessboard_get_piecetype(cb, from) == PAWN);
    assert(_chessboard_get_color(cb, from) == cb->to_move);
    assert(_chessboard_get_color(cb, to) != cb->to_move);
    assert(_is_square_legal(from));
    assert(_is_square_legal(to));
    
    bool valid = false;
    int32_t diff = to - from;
    chessboard_color from_color = _chessboard_get_color(cb, from);
    uint32_t from_rank = from / 16; 
    chessboard_piecetype to_piece = _chessboard_get_piecetype(cb, to);
    if (from_color == WHITE)
    {
	valid = ((diff == -16) && (to_piece == EMPTY)) ||
	    ((from_rank == 6) && (diff == -32) && (to_piece == EMPTY)) ||
	    (((diff == -15) || (diff == -17)) && (to_piece != EMPTY));
    }
    else
    {
	valid = ((diff == 16) && (to_piece == EMPTY)) ||
	    ((from_rank == 1) && (diff == 32) && (to_piece == EMPTY)) ||
	    (((diff == 15) || (diff == 17)) && (to_piece != EMPTY));
    }

    return valid;
}
