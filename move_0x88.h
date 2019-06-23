#ifndef MOVE_0X88_H
#define MOVE_0X88_H

#include "chessboard_api.h"
#include "chessboard_0x88.h"
#include <stdint.h>
#include <stdbool.h>

/*
TODO: The flags here are kind of sloppy, but they'll do for now.
 */
struct _move {
    uint32_t from;
    uint32_t to;

    bool is_king;
    bool is_white_kings_rook;
    bool is_white_queens_rook;
    bool is_black_kings_rook;
    bool is_black_queens_rook;
    bool is_castle;
};

void _move_unchecked(chessboard* cb, struct _move* move);
bool _is_move_valid(chessboard* cb, struct _move* move);
bool _is_knight_move_valid(chessboard* cb, struct _move* move);
bool _is_king_move_valid(chessboard* cb, struct _move* move);
bool _is_castle_move_valid(chessboard* cb, struct _move* move);
bool _is_bishop_move_valid(chessboard* cb, struct _move* move);
bool _is_rook_move_valid(chessboard* cb, struct _move* move);
bool _is_queen_move_valid(chessboard* cb, struct _move* move);
bool _is_pawn_move_valid(chessboard* cb, struct _move* move);

bool _is_player_in_check(chessboard* cb, chessboard_color player);
bool _is_square_attacked(chessboard* cb, uint32_t square, chessboard_color attacker);

#endif
