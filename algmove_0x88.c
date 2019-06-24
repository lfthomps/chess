#include "algmove_0x88.h"
#include "chessboard_api.h"
#include "chessboard_0x88.h"
#include "move_0x88.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <stdio.h> // For debugging

bool chessboard_algmove(chessboard* cb, char* move_str)
{
    struct _move move = {};
    bool valid = cb88_is_alg_move_valid(cb, move_str, &move);
    if (valid) cb88_move_unchecked(cb, &move);
    
    // Handle castling (TODO: Clean this up)
    if (move.is_castle) _move_rook_castling(cb, &move);
    if (move.is_king) cb->castle = (struct _castle_rights){false};
    if (move.is_white_kings_rook) cb->castle.white_short = false;
    if (move.is_white_queens_rook) cb->castle.white_long = false;
    if (move.is_black_kings_rook) cb->castle.black_short = false;
    if (move.is_black_queens_rook) cb->castle.black_long = false;

    DEBUG_validate_board(cb);
    return valid;
}

// TODO: Pawn moves are buggy.  In particular, legal captures don't always
// seem to work, but writing them as advances does.  For example, if
// cxd4 should be a legal move and c4 should not, cxd4 is called illegal,
// but c4 is called legal and makes the move cxd4.

// TODO: This has a lot of room for improvement.  At the moment, it's a giant,
// ugly function with many points of exit.  As with everything else, though,
// I want to get it working first and then make it pretty.
//
// TODO: Also, the move struct isn't cleaned up if we return false.  This is
// probably fine, but I'd prefer to always give invalid structs obviously
// invalid properties so that no one uses them by mistake.  
bool cb88_is_alg_move_valid(chessboard* cb, char* move_str, struct _move* move)
{
    bool valid = false;
    chessboard_color color = cb->to_move;

    // TODO: Castling moves could also have extra characters afterwards -
    // this should check if the strings start with "0-0", etc., instead of
    // if they equal "0-0".
    
    // Castling moves have a special syntax, so we handle them separately.
    if (!strcmp(move_str, "O-O") || !strcmp(move_str, "o-o") || !strcmp(move_str, "0-0"))
    {
	switch (color)
	{
	case WHITE:
	    move->from = cb88_get_square(E1);
	    break;
	case BLACK:
	    move->from = cb88_get_square(E8);
	    break;
	}
	move->to = move->from + 2;
	move->is_castle = true;
	move->is_king = true;
	
	return cb88_is_castle_move_valid(cb, move);
    }
    else if (!strcmp(move_str, "O-O-O") || !strcmp(move_str, "o-o-o") || !strcmp(move_str, "0-0-0"))
    {
	switch (color)
	{
	case WHITE:
	    move->from = cb88_get_square(E1);
	    break;
	case BLACK:
	    move->from = cb88_get_square(E8);
	    break;
	}
	move->to = move->from - 2;
	move->is_castle = true;
	move->is_king = true;

	return cb88_is_castle_move_valid(cb, move);
    }

    // clean_str will hold the move with all trailing symbols
    // like '+' and '#' removed.  The longest possible move in
    // standard algebraic notation is then 6 characters (plus one
    // for the null character).  For example, Nf3xe5 or dxe8=Q.
    // The only symbols that should be included are piece types
    // (K, Q, R, B, N), file and rank names and an x or =.  (Note that 
    // castling is handled elsewhere.)
    //
    // A reasonable user might input a fairly long string if
    // they include check/mate, qualitative descriptions like
    // !, ? or +/-, or scores like 1-0.  A reasonable approach
    // seems to be to try to find a move at the start of the string
    // and ignore all trailing text.
    // TODO: Handle white space in move (e.g., N x e4)
    char clean_str[7] = {0};
    int i = 0;
    while ((chessboard_is_piece(move_str[i]) || chessboard_is_file(move_str[i]) ||
	    chessboard_is_rank(move_str[i]) || move_str[i] == 'x' ||
	    move_str[i] == '=') && i < 6)
    {
	clean_str[i] = move_str[i];
	i++;
    }
    int len = strlen(clean_str);
    if (len < 2)
    {
	return false;
    }

    // Pawn moves (TODO: promotions are not handled yet.)
    if (chessboard_is_file(clean_str[0]))
    {
	// Advances are always in the form (file)(rank), like "e4".
	// There might be other characters like '+' or '#' afterwards,
	// but if a move begins with (file)(rank) then it must be
	// a pawn advance.
	if (chessboard_is_rank(clean_str[1]))
	{
	    uint32_t to = cb88_get_square_from_chars(clean_str[0], clean_str[1]);
	    for (int i = 0; i < CB88_MAX_PIECES; i++)
	    {
		// The check of !valid is to make sure we don't overwrite
		// the move struct once we find a legal move.  This loop
		// is made simpler by the fact that pawn advances can't be
		// ambiguous.  That is, there are never two pawns of the
		// same color that can advance to the same square.
		if (!valid && (cb->piecelist[color][i].type == PAWN))
		{
		    move->to = to;
		    move->from = cb->piecelist[color][i].square;
		    valid = cb88_is_pawn_move_valid(cb, move);
		}
	    }
	    return valid;
	}
	// Captures are of the form (file)x(file)(rank), like "exd4".
	// For the moment we are not allowing the shorthand (file)x(file).
	// As with advances, there might be extra characters afterwards,
	// but they can be ignored.

	// TODO: If a pawn capture (such as exd4) is legal, then it can
	// be entered without the caputre syntax.  For example, d4 would
	// make the capture.  This should be fixed.
	if (clean_str[1] == 'x' && len >= 4 &&
		 chessboard_is_file(clean_str[2]) &&
		 chessboard_is_rank(clean_str[3]))
	{
	    uint32_t to = cb88_get_square_from_chars(clean_str[2], clean_str[3]);
	    uint32_t file = (uint32_t)(clean_str[0] - 'a');
	    for (int i = 0; i < CB88_MAX_PIECES; i++)
	    {
		struct _piece piece = cb->piecelist[color][i];
		// Similar to the advance version above.
		// The check of !valid is to make sure we don't overwrite
		// the move struct once we find a legal move.  As with
		// advances, pawn captures can't be ambiguous.

		if (!valid && (piece.type == PAWN) && (cb88_get_file(piece.square) == file))
		{
		    move->to = to;
		    move->from = piece.square;
		    valid = cb88_is_pawn_move_valid(cb, move);
		}
	    }
	    return valid;
	}
    }

    // Piece moves
    // All piece moves are of the form
    // (type)[file_hint][rank_hint][x](file)(rank)
    // plus some possible extra characters at the end, like '+' or '#'.
    //
    // Throughout this code, we use the fact that the string is known
    // to be at least two characters long.
    switch (clean_str[0])
    {
    case 'K':
	valid = _is_alg_piece_move_valid(cb, clean_str, move, KING);
	break;
    case 'Q':
	valid = _is_alg_piece_move_valid(cb, clean_str, move, QUEEN);
	break;
    case 'R':
	valid = _is_alg_piece_move_valid(cb, clean_str, move, ROOK);
	break;
    case 'N':
	valid = _is_alg_piece_move_valid(cb, clean_str, move, KNIGHT);
	break;
    case 'B':
	valid = _is_alg_piece_move_valid(cb, clean_str, move, BISHOP);
	break;
    default:
	break;
    }
    return valid;
}

bool _is_alg_piece_move_valid(chessboard* cb, char* clean_str, struct _move* move, chessboard_piecetype type)
{
    int len = strlen(clean_str);
    assert(len >= 2);
    bool valid = false;

    // The string should already have been cleaned, so all trailing characters
    // have already been removed.  Once cleaned, every piece move has to end
    // with the target square in the form of (file)(rank).  
    if (!(chessboard_is_rank(clean_str[len-1]) && chessboard_is_file(clean_str[len-2])))
    {
	return false;
    }
    move->to = cb88_get_square_from_chars(clean_str[len-2], clean_str[len-1]);

    // Standard algebraic notation allows (and sometimes requires) a specification
    // of which rank or file (or both) the piece starts on.  There are only supposed
    // to be included if necessary, and if either will work then you are supposed
    // to use files, but we will allow either or both as long as the piece is
    // unambiguously specified.  Note that we don't allow two ranks or two files.
    int i = len-3;
    bool has_rank_hint = false;
    uint32_t rank_hint = 0;
    bool has_file_hint = false;
    uint32_t file_hint = 0;
    while (i > 0)
    {
	if (chessboard_is_rank(clean_str[i]) && !has_rank_hint)
	{
	    has_rank_hint = true;
	    rank_hint = 7 - (uint32_t)(clean_str[i] - '1');
	}
	else if (chessboard_is_file(clean_str[i]) && !has_file_hint)
	{
	    has_file_hint = true;
	    file_hint = (uint32_t)(clean_str[i] - 'a');
	}
	else if (clean_str[i] != 'x')
	{
	    // If we get here, then multiple rank hints or
	    // multiple file hints have been given and the string
	    // is malformed.
	    return false;
	}
    }

    // We now need to check each piece of the appropriate color and type and
    // see if it can move to the appropriate square.  Note that
    // !has_rank_hint || (_get_rank(from) == rank_hint) evaluates to true
    // if there is no rank hint or if there is a rank hint and the from square
    // matches the hint.
    chessboard_color color = cb->to_move;
    uint32_t from_final = CB88_MAX_INDEX;
    int moves_found = 0;
    for (int k = 0; k < CB88_MAX_PIECES; k++)
    {
	struct _piece piece = cb->piecelist[color][k];
	uint32_t from = piece.square;
	if (piece.type == type &&
	    (!has_rank_hint || (cb88_get_rank(from) == rank_hint)) &&
	    (!has_file_hint || (cb88_get_file(from) == file_hint)))
	{
	    move->from = from;
	    if (cb88_is_move_valid(cb, move))
	    {
		from_final = from;
		moves_found++;
	    }
	}
    }

    // TODO: It would be nice to be able to tell the user if there is no
    // matching move or too many matching moves.  That would require
    // rewriting these functions with error codes instead of true/false.
    if (moves_found == 1)
    {
	move->from = from_final;
	valid = true;
    }
    return valid;
}
