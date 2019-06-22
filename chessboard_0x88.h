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
void DEBUG_validate_board(chessboard* cb);

chessboard_color _chessboard_get_color(chessboard* cb, uint32_t square);
chessboard_piecetype _chessboard_get_piecetype(chessboard* cb, uint32_t square);

uint32_t _get_internal_square(chessboard_square square);
bool _is_square_legal(uint32_t square);

int _set_square(chessboard* cb, uint32_t square, chessboard_piecetype type, chessboard_color color);
void _clear_square(chessboard* cb, uint32_t square);
