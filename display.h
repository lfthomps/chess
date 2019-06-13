#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "chessboard.h"

#define BUFFER_SIZE 2367

int fill_line(char* buffer, int buffer_index);
int fill_interior_line(char* buffer, int buffer_index, bool light_square);
int fill_colored_interior_line(char* buffer, int buffer_index, bool light_square);
int fill_rank(char* buffer, int buffer_index, bool light_square);
int fill_board(char* buffer, int buffer_index);

void set_square_bold_red(char* buffer, int ind);
void set_square_bold_green(char* buffer, int ind);

void draw_chessboard(char* buffer, chessboard* cb);

#endif
