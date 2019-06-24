#ifndef ALGMOVE_0X88_H
#define ALGMOVE_0X88_H

#include "chessboard_api.h"
#include "chessboard_0x88.h"
#include "move_0x88.h"
#include <stdint.h>
#include <stdbool.h>

bool cb88_is_alg_move_valid(chessboard* cb, char* move_str, struct _move* move);
bool _is_alg_piece_move_valid(chessboard* cb, char* clean_str, struct _move* move, chessboard_piecetype type);

#endif
