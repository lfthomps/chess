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
 
TODO: All of the chessboard_api functions that take a chessboard_square
as an argument have a corresponding function with a leading underscore
that takes an index from the 0x88 board representation instead.  This 
is so that internal functions can take advantage of these functions
without wasting time translating to and from chessboard_squares. 
Unfortunately, there is enough code here that it seemed prudent to 
split it into multiple source files, so the leading underscore naming
convention is no longer appropriate.  These should be renamed at some
point.  
*/

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
	for (int j = 0; j < CB88_MAX_PIECES; j++)
	{
	    struct _piece piece = cb->piecelist[i][j];
	    printf("Piece %d:\n", j);
	    DEBUG_print_piece(&piece);
	}
    }
}

void DEBUG_print_board(chessboard* cb)
{
    for (uint32_t i = 0; i < CB88_MAX_INDEX; i++)
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

    for (uint32_t square = 0; square < CB88_MAX_INDEX; square++)
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
		(cb->board[square] <= &cb->piecelist[1][CB88_MAX_INDEX-1]);
	    if (!valid) printf("Square %d has invalid address %p\n", square, cb->board[square]);
	    assert(valid);
	    
	    struct _piece piece = *(cb->board[square]);
	    valid = (piece.square == square);
	    if (!valid) printf("board[%d] points to piece with square %d\n", square, piece.square);
	    assert(valid);
	    
	    // WARNING: This relies on the fact that types are ordered
	    // such that valid pieces are between EMPTY and
	    // CHESSBOARD_MAX_PIECETYPE.  If that enum is reordered,
	    // this will have to change.  
	    valid = ((piece.type > EMPTY) &&
		     (piece.type < CHESSBOARD_MAX_PIECETYPE));
	    if (!valid) printf("Square %d has invalid piecetype %d\n", square, piece.type);
	    assert(valid);
	}
    }
    for (chessboard_color color = WHITE; color < CHESSBOARD_MAX_COLOR; color++)
    {
	for (int i = 0; i < CB88_MAX_PIECES; i++)
	{
	    struct _piece piece = cb->piecelist[color][i];
	    if (piece.square ==	CB88_MAX_INDEX)
	    {
		valid = (piece.type == EMPTY);
		if (!valid)
		{
		    printf("Piecelist[%d][%d] is empty but has non-empty type %d\n", color, i, piece.type);
		    DEBUG_print_piece(&piece);
		}
		assert(valid);

		valid = (piece.color == CHESSBOARD_MAX_COLOR);
		if (!valid)
		{
		    printf("Piecelist[%d][%d] is empty but has color %d (should be CHESSBOARD_MAX_COLOR)\n", color, i, piece.color);
		    DEBUG_print_piece(&piece);
		}
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
	for (int index = 0; index < CB88_MAX_INDEX; index++)
	{
	    cb->board[index] = 0;
	}
	for (int color = WHITE; color < CHESSBOARD_MAX_COLOR; color++)
	{
	    for (int piece = 0; piece < CB88_MAX_PIECES; piece++)
	    {
		cb->piecelist[color][piece] =
		    (struct _piece){.color=CHESSBOARD_MAX_COLOR,
				    .type=EMPTY,
				    .square=CB88_MAX_INDEX};
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
    cb->castle = (struct _castle_rights){true, true, true, true};
    
    for (enum chessboard_square square = A7; square < A6; square++)
    {
	uint32_t index = cb88_get_square(square);
	cb88_set_square(cb, index, PAWN, BLACK);
    }
    for (enum chessboard_square square = A2; square < A1; square++)
    {
	uint32_t index = cb88_get_square(square);
	cb88_set_square(cb, index, PAWN, WHITE);
    }
    cb88_set_square(cb, cb88_get_square(A8), ROOK, BLACK);
    cb88_set_square(cb, cb88_get_square(B8), KNIGHT, BLACK);
    cb88_set_square(cb, cb88_get_square(C8), BISHOP, BLACK);
    cb88_set_square(cb, cb88_get_square(D8), QUEEN, BLACK);
    cb88_set_square(cb, cb88_get_square(E8), KING, BLACK);
    cb88_set_square(cb, cb88_get_square(F8), BISHOP, BLACK);
    cb88_set_square(cb, cb88_get_square(G8), KNIGHT, BLACK);
    cb88_set_square(cb, cb88_get_square(H8), ROOK, BLACK);
    cb88_set_square(cb, cb88_get_square(A1), ROOK, WHITE);
    cb88_set_square(cb, cb88_get_square(B1), KNIGHT, WHITE);
    cb88_set_square(cb, cb88_get_square(C1), BISHOP, WHITE);
    cb88_set_square(cb, cb88_get_square(D1), QUEEN, WHITE);
    cb88_set_square(cb, cb88_get_square(E1), KING, WHITE);
    cb88_set_square(cb, cb88_get_square(F1), BISHOP, WHITE);
    cb88_set_square(cb, cb88_get_square(G1), KNIGHT, WHITE);
    cb88_set_square(cb, cb88_get_square(H1), ROOK, WHITE);

    DEBUG_validate_board(cb);
}

chessboard_piecetype chessboard_get_piecetype(chessboard* cb, chessboard_square square)
{
    uint32_t index = cb88_get_square(square);
    return cb->board[index] ? cb->board[index]->type : EMPTY;
}

chessboard_piecetype cb88_get_piecetype(chessboard* cb, uint32_t square)
{
    return cb->board[square] ? cb->board[square]->type : EMPTY;
}

chessboard_color chessboard_get_color(chessboard* cb, chessboard_square square)
{
    uint32_t index = cb88_get_square(square);
    return cb->board[index] ? cb->board[index]->color : CHESSBOARD_MAX_COLOR;
}

chessboard_color cb88_get_color(chessboard* cb, uint32_t square)
{
    return cb->board[square] ? cb->board[square]->color : CHESSBOARD_MAX_COLOR;
}

chessboard_color chessboard_get_current_player(chessboard *cb)
{
    return cb->to_move;
}

void chessboard_switch_current_player(chessboard *cb)
{
    // WARNING: This relies on the fact that WHITE and BLACK are 0 and
    // 1.  If other colors are used for some reason (maybe to indicate
    // an error) then this will cause problems.
    cb->to_move = !(cb->to_move);
}

uint32_t cb88_get_square(chessboard_square square)
{
    assert(square >= A8);
    assert(square < CHESSBOARD_MAX_SQUARE);
    return internal_squares[square];
}

uint32_t cb88_get_square_from_chars(char file, char rank)
{
    return (7 - (uint32_t)(rank - '1')) * 16 + (uint32_t)(file - 'a');
}

bool cb88_is_square_legal(uint32_t square)
{
    return !(square & 0x88);
}

uint32_t cb88_get_file(uint32_t square)
{
    return square & 0xF;
}

uint32_t cb88_get_rank(uint32_t square)
{
    return (square >> 4) & 0xF;
}

bool chessboard_is_rank(char ch)
{
    return (ch >= '1') && (ch <= '8');
}

bool chessboard_is_file(char ch)
{
    return (ch >= 'a') && (ch <= 'h');
}

bool chessboard_is_piece(char ch)
{
    return (ch == 'K') || (ch == 'Q') || (ch == 'R') || (ch == 'N') || (ch == 'B');
}

int cb88_set_square(chessboard* cb, uint32_t square, chessboard_piecetype type, chessboard_color color)
{
    int i = 0;
    while ((i < CB88_MAX_PIECES) && (cb->piecelist[color][i].type != EMPTY)) i++;
    // TODO: Is this check actually doing anything?  Come back and look.  
    if (i >= CB88_MAX_PIECES)
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

void cb88_clear_square(chessboard* cb, uint32_t square)
{
    if (cb->board[square])
    {
	*(cb->board[square]) =
	    (struct _piece){.color=CHESSBOARD_MAX_COLOR,
			    .type=EMPTY,
			    .square=CB88_MAX_INDEX};
    }
    cb->board[square] = NULL;
}
