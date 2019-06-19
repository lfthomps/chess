#include "chessboard_0x88.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include <stdio.h> //For debugging

/*
This is a chessboard implementation using the 0x88 board representation
(see http://www.craftychess.com/hyatt/boardrep.html for reference). 
The board is represented by a length 128 array, which we will visualize
as follows:

  0   1   2   3   4   5   6   7 |   8   9  10  11  12  13  14  15
 16  17  18  19  20  21  22  23 |  24  25  26  27  28  29  30  31
 32  33  34  35  36  37  38  39 |  40  41  42  43  44  45  46  47
 48  49  50  51  52  53  54  55 |  56  57  58  59  60  61  62  63
 64  65  66  67  68  69  70  71 |  72  73  74  75  76  77  78  79
 80  81  82  83  84  85  86  87 |  88  89  90  91  92  93  94  95
 96  97  98  99 100 101 102 103 | 104 105 106 107 108 109 110 111
112 113 114 115 116 117 118 119 | 120 121 122 123 124 125 126 127

The 8x8 block on the left represents legal board squares (where A8=0, 
A7=1, ..., A1=7, ..., H8=112, ..., H1=119) and the 8x8 block on the 
right represents illegal board squares.  One of the big advantages
of this representation is that it is very easy to check if a square
is legal.  If (index & 0x88) is 0, then the square is legal, and if
it is nonzero then the square is illegal.  

Since only the array indices are needed to decide if squares are legal 
or not, the contents of the array are free to hold whatever information
we want.  We will follow a common choice and make the board be an 
array of pointers to pieces.  Squares without pieces (including both
empty and illegal squares) will contain a null pointer.  The pieces 
will be organized in a 2x16 array, where one row holds the white pieces
and the other row holds the black pieces.  This will make it easy to
loop through pieces when necessary.
 */

const int MAX_INDEX = 128;
const int MAX_PIECES = 16;

const uint32_t internal_squares[CHESSBOARD_MAX_SQUARE] =
{0, 1, 2, 3, 4, 5, 6, 7,
 16, 17, 18, 19, 20, 21, 22, 23,
 32, 33, 34, 35, 36, 37, 38, 39,
 48, 49, 50, 51, 52, 53, 54, 55,
 64, 65, 66, 67, 68, 69, 70, 71,
 80, 81, 82, 83, 84, 85, 86, 87,
 96, 97, 98, 99, 100, 101, 102, 103,
 112, 113, 114, 115, 116, 117, 118, 119};

#ifndef NDEBUG
void DEBUG_print_piecelist(chessboard* cb)
{
    printf("Printing Pieces: \n");
    for (int i = 0; i < 2; i++)
    {
	for (int j = 0; j < MAX_PIECES; j++)
	{
	    struct _piece piece = cb->piecelist[i][j];
	    printf("Piece %d:\n", j);
	    DEBUG_print_piece(&piece);
	}
    }
}

void DEBUG_print_board(chessboard* cb)
{
    for (uint32_t i = 0; i < MAX_INDEX; i++)
    {
	if (cb->board[i])
	{
	    printf("Piece on square %u\n", i);
	    DEBUG_print_piece(cb->board[i]);
	}
    }
}

void DEBUG_print_piece(struct _piece* piece)
{
    chessboard_color color = piece->color;
    chessboard_piecetype type = piece->type;
    uint32_t square = piece->square;
    
    switch (color)
    {
    case WHITE:
	printf("\tColor: WHITE");
	break;
    case BLACK:
	printf("\tColor: BLACK");
	break;
    default:
	printf("\tColor: Unknown (%d)", color);
	break;
    }
    switch (type)
    {
    case EMPTY:
	printf("\tType: EMPTY");
	break;
    case PAWN:
	printf("\tType: PAWN");
	break;
    case KNIGHT:
	printf("\tType: KNIGHT");
	break;
    case KING:
	printf("\tType: KING");
	break;
    case QUEEN:
	printf("\tType: QUEEN");
	break;
    case ROOK:
	printf("\tType: ROOK");
	break;
    case BISHOP:
	printf("\tType: BISHOP");
	break;
    default:
	printf("\tType: Unknown (%d)", type);
	break;
    }
    printf("\tSquare: %u", square);
    printf("\tAddress: %p\n", piece);
}

void DEBUG_validate_board(chessboard* cb)
{
    // The asserts in here are double checked so that I can
    // print more useful debug information.  At some point
    // I should write a nicer assert macro to handle this for me.
    bool valid = true;

    for (uint32_t square = 0; square < MAX_INDEX; square++)
    {
	if (cb->board[square])
	{
	    // WARNING: This is meant to check if cb->board[square]
	    // points to an element of cb->piecelist.  Technically,
	    // the behavior of these pointer comparisons is
	    // unspecified if cb->board[square] points outside the
	    // array.  I'm only running the debug code on my computer
	    // though, and it seems to work fine here.  As I understand
	    // it, this works as expected on the vast majority of
	    // systems.
	    valid = (cb->board[square] >= &cb->piecelist[0][0]) &&
		(cb->board[square] <= &cb->piecelist[1][MAX_INDEX]);
	    if (!valid) printf("Square %d has invalid address %p\n", square, cb->board[square]);
	    assert(valid);
	    
	    struct _piece piece = *(cb->board[square]);
	    valid = piece.square == square;
	    if (!valid) printf("board[%d] points to piece with square %d\n", square, piece.square);
	    assert(valid);
	    
	    // WARNING: This relies on the fact that types are ordered
	    // such that valid pieces are between EMPTY and
	    // CHESSBOARD_MAX_PIECETYPE.  If that enum is reordered,
	    // this will have to change.  
	    valid = (piece.type > EMPTY) &&
		(piece.type < CHESSBOARD_MAX_PIECETYPE);
	    if (!valid) printf("Square %d has invalid piecetype %d\n", square, piece.type);
	    assert(valid);
	}
    }
    for (chessboard_color color = WHITE; color < CHESSBOARD_MAX_COLOR; color++)
    {
	for (int i = 0; i < MAX_PIECES; i++)
	{
	    struct _piece piece = cb->piecelist[color][i];
	    if (piece.square ==	MAX_INDEX)
	    {
		valid = piece.type == EMPTY;
		if (!valid) printf("Piecelist[%d][%d] is empty but has non-empty type %d\n", color, i, piece.type);
		DEBUG_print_piece(&piece);
		assert(valid);

		valid = piece.color == CHESSBOARD_MAX_COLOR;
		if (!valid) printf("Piecelist[%d][%d] is empty but has color %d (should be CHESSBOARD_MAX_COLOR)\n", color, i, piece.color);
		assert(valid);
	    }
	    else
	    {
		valid = cb->board[piece.square];
		if (!valid) printf("Piecelist[%d][%d] has square %d, but cb->board[%d] is null\n", color, i, piece.square, piece.square);
		assert(valid);

		valid = cb->board[piece.square] == &(cb->piecelist[color][i]);
		if (!valid) printf("Piecelist[%d][%d] has square %d, but cb->board[%d] points to %p (should be %p)\n", color, i, piece.square, piece.square, cb->board[piece.square], &(cb->piecelist[color][i]));
		assert(valid);
	    }
	}
    }
}
#else // #ifndef NDEBUG
void DEBUG_print_piecelist(chessboard* cb) {}
void DEBUG_print_board(chessboard* cb) {}
void DEBUG_print_piece(struct _piece* piece) {}
void DEBUG_validate_board(chessboard* cb) {}
#endif // #ifndef NDEBUG

chessboard* chessboard_allocate()
{
    chessboard* cb = (chessboard *)malloc(sizeof(chessboard));
    if (cb)
    {
	for (int index = 0; index < MAX_INDEX; index++)
	{
	    cb->board[index] = 0;
	}
	for (int color = WHITE; color < CHESSBOARD_MAX_COLOR; color++)
	{
	    for (int piece = 0; piece < MAX_PIECES; piece++)
	    {
		cb->piecelist[color][piece] =
		    (struct _piece){.color=CHESSBOARD_MAX_COLOR,
				    .type=EMPTY,
				    .square=MAX_INDEX};
	    }
	}
	cb->to_move = CHESSBOARD_MAX_COLOR;
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
    free(cb);
}

void chessboard_initialize_board(chessboard* cb)
{
    cb->to_move = WHITE;
    
    for (enum chessboard_square square = A7; square < A6; square++)
    {
	uint32_t index = _get_internal_square(square);
	_set_square(cb, index, PAWN, BLACK);
    }
    for (enum chessboard_square square = A2; square < A1; square++)
    {
	uint32_t index = _get_internal_square(square);
	_set_square(cb, index, PAWN, WHITE);
    }
    _set_square(cb, _get_internal_square(A8), ROOK, BLACK);
    _set_square(cb, _get_internal_square(B8), KNIGHT, BLACK);
    _set_square(cb, _get_internal_square(C8), BISHOP, BLACK);
    _set_square(cb, _get_internal_square(D8), QUEEN, BLACK);
    _set_square(cb, _get_internal_square(E8), KING, BLACK);
    _set_square(cb, _get_internal_square(F8), BISHOP, BLACK);
    _set_square(cb, _get_internal_square(G8), KNIGHT, BLACK);
    _set_square(cb, _get_internal_square(H8), ROOK, BLACK);
    _set_square(cb, _get_internal_square(A1), ROOK, WHITE);
    _set_square(cb, _get_internal_square(B1), KNIGHT, WHITE);
    _set_square(cb, _get_internal_square(C1), BISHOP, WHITE);
    _set_square(cb, _get_internal_square(D1), QUEEN, WHITE);
    _set_square(cb, _get_internal_square(E1), KING, WHITE);
    _set_square(cb, _get_internal_square(F1), BISHOP, WHITE);
    _set_square(cb, _get_internal_square(G1), KNIGHT, WHITE);
    _set_square(cb, _get_internal_square(H1), ROOK, WHITE);

    DEBUG_validate_board(cb);
}

chessboard_piecetype chessboard_get_piecetype(chessboard* cb, chessboard_square square)
{
    uint32_t index = _get_internal_square(square);
    return cb->board[index] ? cb->board[index]->type : EMPTY;
}

chessboard_piecetype _chessboard_get_piecetype(chessboard* cb, uint32_t square)
{
    return cb->board[square] ? cb->board[square]->type : EMPTY;
}

chessboard_color chessboard_get_color(chessboard* cb, chessboard_square square)
{
    uint32_t index = _get_internal_square(square);
    return cb->board[index] ? cb->board[index]->color : CHESSBOARD_MAX_COLOR;
}

chessboard_color _chessboard_get_color(chessboard* cb, uint32_t square)
{
    return cb->board[square] ? cb->board[square]->color : CHESSBOARD_MAX_COLOR;
}

chessboard_color chessboard_get_current_player(chessboard *cb)
{
    return cb->to_move;
}

uint32_t _get_internal_square(chessboard_square square)
{
    assert(square >= A8);
    assert(square < CHESSBOARD_MAX_SQUARE);
    return internal_squares[square];
}

bool _is_square_legal(uint32_t square)
{
    return !(square & 0x88);
}

int _set_square(chessboard* cb, uint32_t square, chessboard_piecetype type, chessboard_color color)
{
    int i = 0;
    while ((i < MAX_PIECES) && (cb->piecelist[color][i].type != EMPTY)) i++;
    if (i >= MAX_PIECES)
    {
	printf("DEBUG: Added more than 16 pieces to board\n");
	return -1;
    }
    cb->piecelist[color][i] = (struct _piece){.color=color,
					      .type=type,
					      .square=square};
    cb->board[square] = &(cb->piecelist[color][i]);

    return 0;
}

void _clear_square(chessboard* cb, uint32_t square)
{
    if (cb->board[square])
    {
	*(cb->board[square]) =
	    (struct _piece){.color=CHESSBOARD_MAX_COLOR,
			    .type=EMPTY,
			    .square=MAX_INDEX};
    }
    cb->board[square] = NULL;
}

void _move_unchecked(chessboard* cb, uint32_t from, uint32_t to)
{
    _clear_square(cb, to);
    cb->board[to] = cb->board[from];
    cb->board[to]->square = to;
    cb->board[from] = NULL;
}

bool chessboard_move(chessboard* cb, chessboard_square from, chessboard_square to)
{
    uint32_t from_index = _get_internal_square(from);
    uint32_t to_index = _get_internal_square(to);
    bool valid = _is_move_valid(cb, from_index, to_index);
    if (valid) _move_unchecked(cb, from_index, to_index);
    DEBUG_validate_board(cb);
    return valid;
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
	case BISHOP:
	case ROOK:
	case QUEEN:
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
