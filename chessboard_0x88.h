#include "chessboard_api.h"
#include <stdint.h>

struct _piece {
    chessboard_color color;
    chessboard_piecetype type;
    uint32_t square;
};

struct chessboard {
    struct _piece * board[128];
    struct _piece piecelist[2][16];
    chessboard_color to_move;
};

void DEBUG_print_piecelist(chessboard* cb);
void DEBUG_print_board(chessboard* cb);
void DEBUG_print_piece(struct _piece* piece);

uint32_t _get_internal_square(chessboard_square square);
bool _is_square_legal(uint32_t square);

int _set_square(chessboard* cb, uint32_t square, chessboard_piecetype type, chessboard_color color);
void _clear_square(chessboard* cb, uint32_t square);

void _move_unchecked(chessboard* cb, uint32_t from, uint32_t to);
bool _is_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_knight_move_valid(chessboard* cb, uint32_t from, uint32_t to);
