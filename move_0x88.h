#include "chessboard_api.h"
#include "chessboard_0x88.h"
#include <stdint.h>

void _move_unchecked(chessboard* cb, uint32_t from, uint32_t to);
bool _is_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_knight_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_king_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_bishop_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_rook_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_queen_move_valid(chessboard* cb, uint32_t from, uint32_t to);
bool _is_pawn_move_valid(chessboard* cb, uint32_t from, uint32_t to);
