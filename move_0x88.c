#include "move_0x88.h"
#include <assert.h>
#include <stdint.h>

#include <stdio.h> //For debugging

bool chessboard_move(chessboard* cb, chessboard_square from, chessboard_square to)
{
    struct _move move =
	(struct _move){.from=_get_internal_square(from),
		       .to=_get_internal_square(to)};
    bool valid = _is_move_valid(cb, &move);
    if (valid) _move_unchecked(cb, &move);
    if (move.is_castle)
    {
	struct _move rook_move = {};
	switch (to)
	{
	case G1:
	case G8:
	    assert(chessboard_get_piecetype(cb, to+1) == ROOK && "Invalid short castle move attempted");
	    rook_move = (struct _move){.from=move.to+1,
				       .to=move.to-1};
	    _move_unchecked(cb, &rook_move);
	    break;
	case C1:
	case C8:
	    assert(chessboard_get_piecetype(cb, to-2) == ROOK && "Invalid long castle move attempted");
	    rook_move = (struct _move){.from=move.to-2,
				       .to=move.to+1};
	    _move_unchecked(cb, &rook_move);
	    break;
	default:
	    assert(false && "Invalid castle move attempted");
	    break;
	}
    }

    if (move.is_king) cb->castle = (struct _castle_rights){false};
    if (move.is_white_kings_rook) cb->castle.white_short = false;
    if (move.is_white_queens_rook) cb->castle.white_long = false;
    if (move.is_black_kings_rook) cb->castle.black_short = false;
    if (move.is_black_queens_rook) cb->castle.black_long = false;

    DEBUG_validate_board(cb);
    return valid;
}

void _move_unchecked(chessboard* cb, struct _move* move)
{
    _clear_square(cb, move->to);
    cb->board[move->to] = cb->board[move->from];
    cb->board[move->to]->square = move->to;
    cb->board[move->from] = NULL;
}

bool _is_move_valid(chessboard* cb, struct _move* move)
{
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));
    
    bool valid = false;
    bool from_color_valid = (_chessboard_get_color(cb, move->from) == cb->to_move);
    bool to_color_valid = (_chessboard_get_color(cb, move->to) != cb->to_move);
    if (from_color_valid && to_color_valid)
    {
	switch (_chessboard_get_piecetype(cb, move->from))
	{
	case KNIGHT:
	    valid = _is_knight_move_valid(cb, move);
	    break;
	case KING:
	    valid = _is_king_move_valid(cb, move);
	    break;
	case BISHOP:
	    valid = _is_bishop_move_valid(cb, move);
	    break;
	case ROOK:
	    valid = _is_rook_move_valid(cb, move);
	    break;
	case QUEEN:
	    valid = _is_bishop_move_valid(cb, move) || _is_rook_move_valid(cb, move);
	    break;
	case PAWN:
	    valid = _is_pawn_move_valid(cb, move);
	    break;
	default:
	    break;
	}
    }

    return valid;
}

bool _is_knight_move_valid(chessboard* cb, struct _move* move)
{
    assert(_chessboard_get_piecetype(cb, move->from) == KNIGHT);
    assert(_chessboard_get_color(cb, move->from) == cb->to_move);
    assert(_chessboard_get_color(cb, move->to) != cb->to_move);
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));

    bool valid = false;
    int32_t diff = move->to - move->from;
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

bool _is_king_move_valid(chessboard* cb, struct _move* move)
{
    assert(_chessboard_get_piecetype(cb, move->from) == KING);
    assert(_chessboard_get_color(cb, move->from) == cb->to_move);
    assert(_chessboard_get_color(cb, move->to) != cb->to_move);
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));

    bool valid = false;
    int32_t diff = move->to - move->from;
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
    case 2:
    case -2:
	valid = _is_castle_move_valid(cb, move);
	break;
    default:
	break;
    }

    if (valid) move->is_king = true;
    return valid;
}

bool _is_castle_move_valid(chessboard* cb, struct _move* move)
{
    bool valid = false;
    int32_t diff = move->to - move->from;
    switch (_chessboard_get_color(cb, move->from))
    {
    case WHITE:
	if (diff == 2)
	{
	    valid = (cb->castle.white_short &&
		     move->from == _get_internal_square(E1) &&
		     _chessboard_get_piecetype(cb, move->to) == EMPTY &&
		     _chessboard_get_piecetype(cb, move->from+1) == EMPTY &&
		     !_is_square_attacked(cb, move->from, BLACK) &&
		     !_is_square_attacked(cb, move->from+1, BLACK) &&
		     !_is_square_attacked(cb, move->to, BLACK));
	}
	else
	{
	    assert(diff == -2 && "_is_castle_move_valid was passed a king move that wasn't 2 squares right or left");
	    valid = (cb->castle.white_long &&
		     move->from == _get_internal_square(E1) &&
		     _chessboard_get_piecetype(cb, move->to) == EMPTY &&
		     _chessboard_get_piecetype(cb, move->from-1) == EMPTY &&
		     !_is_square_attacked(cb, move->from, BLACK) &&
		     !_is_square_attacked(cb, move->from-1, BLACK) &&
		     !_is_square_attacked(cb, move->to, BLACK));
	}
	break;
    case BLACK:
	if (diff == 2)
	{
	    valid = (cb->castle.black_short &&
		     move->from == _get_internal_square(E8) &&
		     _chessboard_get_piecetype(cb, move->to) == EMPTY &&
		     _chessboard_get_piecetype(cb, move->from+1) == EMPTY &&
		     !_is_square_attacked(cb, move->from, WHITE) &&
		     !_is_square_attacked(cb, move->from+1, WHITE) &&
		     !_is_square_attacked(cb, move->to, WHITE));
	}
	else
	{
	    assert(diff == -2 && "_is_castle_move_valid was passed a king move that wasn't 2 squares right or left");
	    valid = (cb->castle.white_long &&
		     move->from == _get_internal_square(E8) &&
		     _chessboard_get_piecetype(cb, move->to) == EMPTY &&
		     _chessboard_get_piecetype(cb, move->from-1) == EMPTY &&
		     !_is_square_attacked(cb, move->from, WHITE) &&
		     !_is_square_attacked(cb, move->from-1, WHITE) &&
		     !_is_square_attacked(cb, move->to, WHITE));
	}
	break;
    default:
	assert(false && "Invalid piece color");
	break;
    }

    move->is_castle = valid;
    return valid;
}

bool _is_bishop_move_valid(chessboard* cb, struct _move* move)
{
    assert(_chessboard_get_piecetype(cb, move->from) == BISHOP || _chessboard_get_piecetype(cb, move->from) == QUEEN);
    assert(_chessboard_get_color(cb, move->from) == cb->to_move);
    assert(_chessboard_get_color(cb, move->to) != cb->to_move);
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));

    bool valid = false;
    int32_t diff = move->to - move->from;
    int32_t bishop_directions[2] = {17, 15};
    for (int i = 0; i < 2; i++)
    {
	int32_t direction = (move->to > move->from) ? bishop_directions[i] : -bishop_directions[i];
	if (diff % direction == 0)
	{
	    uint32_t test = move->from;
	    while (test != move->to)
	    {
		test += direction;
		if (_chessboard_get_piecetype(cb, test) != EMPTY) break;
	    }
	    if (test == move->to) valid = true;
	}
    }

    return valid;
}

bool _is_rook_move_valid(chessboard* cb, struct _move* move)
{
    assert(_chessboard_get_piecetype(cb, move->from) == ROOK || _chessboard_get_piecetype(cb, move->from) == QUEEN);
    assert(_chessboard_get_color(cb, move->from) == cb->to_move);
    assert(_chessboard_get_color(cb, move->to) != cb->to_move);
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));

    bool valid = false;
    int32_t diff = move->to - move->from;
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
	int32_t direction = (move->to > move->from) ? 16 : -16;
	if (diff % direction == 0)
	{
	    uint32_t test = move->from;
	    while (test != move->to)
	    {
		test += direction;
		if (_chessboard_get_piecetype(cb, test) != EMPTY) break;
	    }
	    if (test == move->to) valid = true;
	}
    }
    else if ((move->to / 16) == (move->from / 16))
    {
	int32_t direction = (move->to > move->from) ? 1 : -1;
	uint32_t test = move->from;
	while (test != move->to)
	{
	    test += direction;
	    if (_chessboard_get_piecetype(cb, test) != EMPTY) break;
	}
	if (test == move->to) valid = true;
    }

    // NOTE: The is_color_side_rook flags are only used for updating
    // castling rights, so it's ok to be sloppy when we set these to
    // true.  For instance, if any rook is leaving H1 (whether it's
    // white or black) we know that white can't castle short.
    if (valid && move->from == _get_internal_square(H1))
    {
	move->is_white_kings_rook = true;
    }
    else if (valid && move->from == _get_internal_square(H8))
    {
	move->is_black_kings_rook = true;
    }
    else if (valid && move->from == _get_internal_square(A1))
    {
	move->is_white_queens_rook = true;
    }
    else if (valid && move->from == _get_internal_square(A8))
    {
	move->is_black_queens_rook = true;
    }
    return valid;
}

bool _is_pawn_move_valid(chessboard* cb, struct _move* move)
{
    assert(_chessboard_get_piecetype(cb, move->from) == PAWN);
    assert(_chessboard_get_color(cb, move->from) == cb->to_move);
    assert(_chessboard_get_color(cb, move->to) != cb->to_move);
    assert(_is_square_legal(move->from));
    assert(_is_square_legal(move->to));
    
    bool valid = false;
    int32_t diff = move->to - move->from;
    chessboard_color from_color = _chessboard_get_color(cb, move->from);
    uint32_t from_rank = move->from / 16; 
    chessboard_piecetype to_piece = _chessboard_get_piecetype(cb, move->to);
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

bool _is_player_in_check(chessboard* cb, chessboard_color player)
{
    int i = 0;
    struct _piece piece = cb->piecelist[player][i];
    while (piece.type != KING)
    {
	i++;
	piece = cb->piecelist[player][i];
	assert(piece.color == player && "No king in piecelist");
    }
    return _is_square_attacked(cb, piece.square, !player);
}

/*
This functions primary purpose is to help test if a player is in check.
In that case, passing the color is unnecessary because we can find the
color by looking at the piece (the king) on the given square.  However,
we also need to check if some empty squares are under attack when 
castling, and it is necessary to pass a color in those cases.  
 */
bool _is_square_attacked(chessboard* cb, uint32_t square, chessboard_color attacker)
{
    bool valid = false;
    for (int i = 0; i < MAX_PIECES_0X88; i++)
    {
	struct _piece piece = cb->piecelist[attacker][i];
	struct _move move = (struct _move){.from=piece.square,
					   .to=square};
	if (_is_move_valid(cb, &move))
	{
	    valid = true;
	    break;
	}
    }

    return valid;
}
