#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "chessboard.h"

// square_width does not include the outer boundaries of the board.
//
// The board width is the width of 8 squares, plus two more for the
// outer boundaries.  
// It must be true that:
// board_width = 8 * square_width + 2;
//
// The board consists of 8 ranks, each with 3 interior lines, as well as
// a filled line at the top and the bottom.  One of the interior lines
// in each rank has extra escape codes for formatting - this is so that
// the pieces will be easier to read.  In particular, each square needs
// 4 characters to make the middle character bold (or to turn off bold),
// 5 characters to change the color of the middle character, and 4
// characters to reset the default font afterwards.  This means that
// the board requires (board_width + 1) characters for the top and
// bottom boundaries, 16*(board_width + 1) characters for the non-colored
// lines in each rank, and 8*(board_width + 8*14) characters for the
// colored lines in each rank.  Therefore, 
// It must be true that:
// BUFFER_SIZE > 18 * (board_width + 1) + 8 * (board_width + 1 + 8*13).
// (The +1 is to account for newlines.)
int square_width = 7;
int board_width = 58;
const int BUFFER_SIZE = 2367; 

typedef enum display_square {
  DISPLAY_A8 = 131,  DISPLAY_B8 = 151,  DISPLAY_C8 = 171,  DISPLAY_D8 = 191,
  DISPLAY_E8 = 211,  DISPLAY_F8 = 231,  DISPLAY_G8 = 251,  DISPLAY_H8 = 271,
  DISPLAY_A7 = 412,  DISPLAY_B7 = 432,  DISPLAY_C7 = 452,  DISPLAY_D7 = 472,
  DISPLAY_E7 = 492,  DISPLAY_F7 = 512,  DISPLAY_G7 = 532,  DISPLAY_H7 = 552,
  DISPLAY_A6 = 693,  DISPLAY_B6 = 713,  DISPLAY_C6 = 733,  DISPLAY_D6 = 753,
  DISPLAY_E6 = 773,  DISPLAY_F6 = 793,  DISPLAY_G6 = 813,  DISPLAY_H6 = 833,
  DISPLAY_A5 = 974,  DISPLAY_B5 = 994,  DISPLAY_C5 = 1014, DISPLAY_D5 = 1034,
  DISPLAY_E5 = 1054, DISPLAY_F5 = 1074, DISPLAY_G5 = 1094, DISPLAY_H5 = 1114,
  DISPLAY_A4 = 1255, DISPLAY_B4 = 1275, DISPLAY_C4 = 1295, DISPLAY_D4 = 1315,
  DISPLAY_E4 = 1335, DISPLAY_F4 = 1355, DISPLAY_G4 = 1375, DISPLAY_H4 = 1395,
  DISPLAY_A3 = 1536, DISPLAY_B3 = 1556, DISPLAY_C3 = 1576, DISPLAY_D3 = 1596,
  DISPLAY_E3 = 1616, DISPLAY_F3 = 1636, DISPLAY_G3 = 1656, DISPLAY_H3 = 1676,
  DISPLAY_A2 = 1817, DISPLAY_B2 = 1837, DISPLAY_C2 = 1857, DISPLAY_D2 = 1877,
  DISPLAY_E2 = 1897, DISPLAY_F2 = 1917, DISPLAY_G2 = 1937, DISPLAY_H2 = 1957,
  DISPLAY_A1 = 2098, DISPLAY_B1 = 2118, DISPLAY_C1 = 2138, DISPLAY_D1 = 2158,
  DISPLAY_E1 = 2178, DISPLAY_F1 = 2198, DISPLAY_G1 = 2218, DISPLAY_H1 = 2238,
} square;

int fill_line(char* buffer, int buffer_index);
int fill_interior_line(char* buffer, int buffer_index, int light_square);
int fill_colored_interior_line(char* buffer, int buffer_index, int light_square);
int fill_rank(char* buffer, int buffer_index, int light_square);
int fill_board(char* buffer, int buffer_index);

void set_square_bold_red(char* buffer, square ind);
void set_square_bold_green(char* buffer, square ind);

int main(int argc, char* argv[])
{
  char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
  if (buffer)
    {
      for (int i = 0; i < BUFFER_SIZE; i++)
	{
	  buffer[i] = '\0';
	}
    }
  else
    {
      printf("Failed to allocate buffer\n");
      return -1;
    }

  chessboard cb = {};
  chessboard_initialize_board(&cb);

  int buffer_index = 0;
  buffer_index = fill_board(buffer, buffer_index);

  // This is just temporary so that I can see what the board will
  // look like.  Eventually we will take a board representation
  // and use it to fill in the pieces for the display.  
  buffer[DISPLAY_A8] = 'R'; set_square_bold_red(buffer, DISPLAY_A8);
  buffer[DISPLAY_B8] = 'N'; set_square_bold_red(buffer, DISPLAY_B8);
  buffer[DISPLAY_C8] = 'B'; set_square_bold_red(buffer, DISPLAY_C8);
  buffer[DISPLAY_D8] = 'Q'; set_square_bold_red(buffer, DISPLAY_D8);
  buffer[DISPLAY_E8] = 'K'; set_square_bold_red(buffer, DISPLAY_E8);
  buffer[DISPLAY_F8] = 'B'; set_square_bold_red(buffer, DISPLAY_F8);
  buffer[DISPLAY_G8] = 'N'; set_square_bold_red(buffer, DISPLAY_G8);
  buffer[DISPLAY_H8] = 'R'; set_square_bold_red(buffer, DISPLAY_H8);
  buffer[DISPLAY_A7] = 'P'; set_square_bold_red(buffer, DISPLAY_A7);
  buffer[DISPLAY_B7] = 'P'; set_square_bold_red(buffer, DISPLAY_B7);
  buffer[DISPLAY_C7] = 'P'; set_square_bold_red(buffer, DISPLAY_C7);
  buffer[DISPLAY_D7] = 'P'; set_square_bold_red(buffer, DISPLAY_D7);
  buffer[DISPLAY_E7] = 'P'; set_square_bold_red(buffer, DISPLAY_E7);
  buffer[DISPLAY_F7] = 'P'; set_square_bold_red(buffer, DISPLAY_F7);
  buffer[DISPLAY_G7] = 'P'; set_square_bold_red(buffer, DISPLAY_G7);
  buffer[DISPLAY_H7] = 'P'; set_square_bold_red(buffer, DISPLAY_H7);
  buffer[DISPLAY_A2] = 'P'; set_square_bold_green(buffer, DISPLAY_A2);
  buffer[DISPLAY_B2] = 'P'; set_square_bold_green(buffer, DISPLAY_B2);
  buffer[DISPLAY_C2] = 'P'; set_square_bold_green(buffer, DISPLAY_C2);
  buffer[DISPLAY_D2] = 'P'; set_square_bold_green(buffer, DISPLAY_D2);
  buffer[DISPLAY_E2] = 'P'; set_square_bold_green(buffer, DISPLAY_E2);
  buffer[DISPLAY_F2] = 'P'; set_square_bold_green(buffer, DISPLAY_F2);
  buffer[DISPLAY_G2] = 'P'; set_square_bold_green(buffer, DISPLAY_G2);
  buffer[DISPLAY_H2] = 'P'; set_square_bold_green(buffer, DISPLAY_H2);
  buffer[DISPLAY_A1] = 'R'; set_square_bold_green(buffer, DISPLAY_A1);
  buffer[DISPLAY_B1] = 'N'; set_square_bold_green(buffer, DISPLAY_B1);
  buffer[DISPLAY_C1] = 'B'; set_square_bold_green(buffer, DISPLAY_C1);
  buffer[DISPLAY_D1] = 'Q'; set_square_bold_green(buffer, DISPLAY_D1);
  buffer[DISPLAY_E1] = 'K'; set_square_bold_green(buffer, DISPLAY_E1);
  buffer[DISPLAY_F1] = 'B'; set_square_bold_green(buffer, DISPLAY_F1);
  buffer[DISPLAY_G1] = 'N'; set_square_bold_green(buffer, DISPLAY_G1);
  buffer[DISPLAY_H1] = 'R'; set_square_bold_green(buffer, DISPLAY_H1);

  printf("\n");
  printf("%s\n", buffer);

  free(buffer);

  return 0;
}

int fill_line(char* buffer, int buffer_index)
{
  int stop_index = buffer_index + board_width;
  for ( ; buffer_index < stop_index; buffer_index++)
    {
      buffer[buffer_index] = '#';
    }
  buffer[buffer_index++] = '\n';

  return buffer_index;
}

int fill_interior_line(char* buffer, int buffer_index, int light_square)
{
  buffer[buffer_index++] = '#';
  for (int square = 0; square < 8; square++)
    {
      if (light_square)
	{
	  for (int i = 0; i < square_width; i++)
	    {
	      buffer[buffer_index++] = '/';
	    }
	}
      else
	{
	  for (int i = 0; i < square_width; i++)
	    {
	      buffer[buffer_index++] = ' ';
	    }
	}
      light_square = !(light_square);
    }
  buffer[buffer_index++] = '#';
  buffer[buffer_index++] = '\n';

  return buffer_index;
}

int fill_colored_interior_line(char* buffer, int buffer_index, int light_square)
{
  buffer[buffer_index++] = '#';
  for (int square = 0; square < 8; square++)
    {
      if (light_square)
	{
	  for (int i = 0; i < square_width / 2; i++)
	    {
	      buffer[buffer_index++] = '/';
	    }
	  // Set text to bold
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '0';
	  buffer[buffer_index++] = 'm';
	  // Set text to colored (still varying this for testing)
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '3';
	  buffer[buffer_index++] = '9';
	  buffer[buffer_index++] = 'm';
	  // Reset default text
	  buffer[buffer_index++] = '/';
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '0';
	  buffer[buffer_index++] = 'm';
	  for (int i = 0; i < square_width / 2; i++)
	    {
	      buffer[buffer_index++] = '/';
	    }
	}
      else
	{
	  for (int i = 0; i < square_width / 2; i++)
	    {
	      buffer[buffer_index++] = ' ';
	    }
	  // Set text to bold
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '0';
	  buffer[buffer_index++] = 'm';
	  // Set text to colored (still varying this for testing)
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '3';
	  buffer[buffer_index++] = '9';
	  buffer[buffer_index++] = 'm';
	  // Reset default text
	  buffer[buffer_index++] = ' ';
	  buffer[buffer_index++] = '\x1B';
	  buffer[buffer_index++] = '[';
	  buffer[buffer_index++] = '0';
	  buffer[buffer_index++] = 'm';
	  for (int i = 0; i < square_width / 2; i++)
	    {
	      buffer[buffer_index++] = ' ';
	    }
	}
      light_square = !(light_square);
    }

  buffer[buffer_index++] = '#';
  buffer[buffer_index++] = '\n';

  return buffer_index;
}

int fill_rank(char* buffer, int buffer_index, int light_square)
{
  buffer_index = fill_interior_line(buffer, buffer_index, light_square);
  buffer_index = fill_colored_interior_line(buffer, buffer_index, light_square);
  buffer_index = fill_interior_line(buffer, buffer_index, light_square);

  return buffer_index;
}

int fill_board(char* buffer, int buffer_index)
{
  int light_square = 1;
  buffer_index = fill_line(buffer, buffer_index);
  for (int rank = 0; rank < 8; rank++)
    {
      buffer_index = fill_rank(buffer, buffer_index, light_square);
      light_square = !(light_square);
    }
  buffer_index = fill_line(buffer, buffer_index);

  return buffer_index;
}

void set_square_bold_red(char* buffer, square ind)
{
  // Change to bold escape code
  buffer[ind-7] = '1';
  // Change to red escape code
  buffer[ind-2] = '1';
}

void set_square_bold_green(char* buffer, square ind)
{
  // Change to bold escape code
  buffer[ind-7] = '1';
  // Change to green escape code
  buffer[ind-2] = '2';
}

void draw_chessboard(char* buffer, chessboard* cb)
{
    // TODO:
}
