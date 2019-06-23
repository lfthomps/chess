#ifndef CHESSBOARD_0X88_H
#define CHESSBOARD_0X88_H

#include "chessboard_api.h"
#include <stdint.h>
#include <stdbool.h>

struct _piece {
    chessboard_color color;
    chessboard_piecetype type;
    uint32_t square;
};

struct _castle_rights {
    bool white_short;
    bool white_long;
    bool black_short;
    bool black_long;
};

struct chessboard {
    struct _piece * board[128];
    struct _piece piecelist[2][16];
    chessboard_color to_move;
    struct _castle_rights castle;
};

#define MAX_INDEX_0X88 128
#define MAX_PIECES_0X88  16

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

#endif
