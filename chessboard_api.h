#ifndef CHESSBOARD_API_H
#define CHESSBOARD_API_H

#include <stdbool.h>

/*
Chessboard API
------------------------------------------------------------------------
We will ultimately use several different representations of chessboards,
including at least 0x88 boards and bitboards.  It is obviously 
impractical for users (such as displays) to keep track of these internal
representations.  Instead, we will insist that all representations 
define a type "chessboard" and all of the following functions.  

It may turn out that some users need access to the internals of a 
representation for optimization reasons (engine evaluations, I've got
my eye on you), but I have a strong preference for keeping "chessboard"
opaque whenever possible.  

Many of these functions need squares on the chessboard, player color or
piece type as input/output.  We will also insist that all such input and output be translated to the following constants.  In particular, 
squares will be labeled by A8=0, B8=1, ..., G1=62, H1=63.  

None of the functions are required to test for valid square, color or
type inputs.  It is up to the caller to make sure that 

0 <= square < CHESSBOARD_MAX_SQUARE,
0 <= color < CHESSBOARD_MAX_COLOR, 
0 <= type < CHESSBOARD_MAX_TYPE
 */

typedef struct chessboard chessboard;

typedef enum chessboard_square {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1,
    CHESSBOARD_MAX_SQUARE,
} chessboard_square;

typedef enum chessboard_color {
    WHITE, BLACK, CHESSBOARD_MAX_COLOR,
} chessboard_color;

typedef enum chessboard_piecetype {
    EMPTY, PAWN, KNIGHT, KING, BISHOP, QUEEN, ROOK, CHESSBOARD_MAX_PIECETYPE,
} chessboard_piecetype;

/*
chessboards should always be created through chessboard_allocate.  
(We may need to add some more allocation methods later for memory
efficiency purposes.)  chessboard_allocate returns a null pointer
if allocation fails.  It is up to the caller to free any chessboards
they allocate using chessboard_free.  
 */
chessboard* chessboard_allocate();
void chessboard_free(chessboard* cb);

/*
chessboard_initialize_board sets up an already allocated chessboard in
the standard starting position (with white to move).
 */
void chessboard_initialize_board(chessboard* cb);

/*
chessboard_get_current_player returns the color of the player who has
the right to move.
 */
chessboard_color chessboard_get_current_player(chessboard *cb);

/*
chessboard_get_piecetype and chessboard_get_color return the 
type and color, respectively, of the piece on square "square".  

A square without a piece always has piecetype EMPTY, but its color
is implementation defined.
 */
chessboard_piecetype chessboard_get_piecetype(chessboard* cb, chessboard_square square);
chessboard_color chessboard_get_color(chessboard* cb, chessboard_square square);

/*
chessboard_move checks if it is legal to move a piece from "from"
 to "to".  If the move is legal, chessboard_move makes it and returns
true.  Otherwise it returns false.

TODO: Do we want to split this into a method that validates if moves 
are legal and another that makes legal moves?  I'm going to leave it
as is until we run into a use case that *really* wants to pre-check
legality.  It seems messy to trust callers to check legality themselves.
 */
bool chessboard_move(chessboard* cb, chessboard_square from, chessboard_square to);

#endif
