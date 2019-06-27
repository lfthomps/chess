#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "chessboard_api.h"

#define BUFFER_SIZE 2367

int display_fill_board(char* buffer, int buffer_index);
void display_draw_chessboard(char* buffer, chessboard* cb);

#endif
