#include <stdio.h>
#include <stdlib.h>

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
const int BUFFER_SIZE = 2367; //Old value: 1535;

typedef enum square {
  A8 = 131,  B8 = 151,  C8 = 171,  D8 = 191,
  E8 = 211,  F8 = 231,  G8 = 251,  H8 = 271,
  A7 = 412,  B7 = 432,  C7 = 452,  D7 = 472,
  E7 = 492,  F7 = 512,  G7 = 532,  H7 = 552,
  A6 = 693,  B6 = 713,  C6 = 733,  D6 = 753,
  E6 = 773,  F6 = 793,  G6 = 813,  H6 = 833,
  A5 = 974,  B5 = 994,  C5 = 1014, D5 = 1034,
  E5 = 1054, F5 = 1074, G5 = 1094, H5 = 1114,
  A4 = 1255, B4 = 1275, C4 = 1295, D4 = 1315,
  E4 = 1335, F4 = 1355, G4 = 1375, H4 = 1395,
  A3 = 1536, B3 = 1556, C3 = 1576, D3 = 1596,
  E3 = 1616, F3 = 1636, G3 = 1656, H3 = 1676,
  A2 = 1817, B2 = 1837, C2 = 1857, D2 = 1877,
  E2 = 1897, F2 = 1917, G2 = 1937, H2 = 1957,
  A1 = 2098, B1 = 2118, C1 = 2138, D1 = 2158,
  E1 = 2178, F1 = 2198, G1 = 2218, H1 = 2238,
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

  int buffer_index = 0;
  buffer_index = fill_board(buffer, buffer_index);

  // This is just temporary so that I can see what the board will
  // look like.  Eventually we will take a board representation
  // and use it to fill in the pieces for the display.  
  buffer[A8] = 'R'; set_square_bold_red(buffer, A8);
  buffer[B8] = 'N'; set_square_bold_red(buffer, B8);
  buffer[C8] = 'B'; set_square_bold_red(buffer, C8);
  buffer[D8] = 'Q'; set_square_bold_red(buffer, D8);
  buffer[E8] = 'K'; set_square_bold_red(buffer, E8);
  buffer[F8] = 'B'; set_square_bold_red(buffer, F8);
  buffer[G8] = 'N'; set_square_bold_red(buffer, G8);
  buffer[H8] = 'R'; set_square_bold_red(buffer, H8);
  buffer[A7] = 'P'; set_square_bold_red(buffer, A7);
  buffer[B7] = 'P'; set_square_bold_red(buffer, B7);
  buffer[C7] = 'P'; set_square_bold_red(buffer, C7);
  buffer[D7] = 'P'; set_square_bold_red(buffer, D7);
  buffer[E7] = 'P'; set_square_bold_red(buffer, E7);
  buffer[F7] = 'P'; set_square_bold_red(buffer, F7);
  buffer[G7] = 'P'; set_square_bold_red(buffer, G7);
  buffer[H7] = 'P'; set_square_bold_red(buffer, H7);
  buffer[A2] = 'P'; set_square_bold_green(buffer, A2);
  buffer[B2] = 'P'; set_square_bold_green(buffer, B2);
  buffer[C2] = 'P'; set_square_bold_green(buffer, C2);
  buffer[D2] = 'P'; set_square_bold_green(buffer, D2);
  buffer[E2] = 'P'; set_square_bold_green(buffer, E2);
  buffer[F2] = 'P'; set_square_bold_green(buffer, F2);
  buffer[G2] = 'P'; set_square_bold_green(buffer, G2);
  buffer[H2] = 'P'; set_square_bold_green(buffer, H2);
  buffer[A1] = 'R'; set_square_bold_green(buffer, A1);
  buffer[B1] = 'N'; set_square_bold_green(buffer, B1);
  buffer[C1] = 'B'; set_square_bold_green(buffer, C1);
  buffer[D1] = 'Q'; set_square_bold_green(buffer, D1);
  buffer[E1] = 'K'; set_square_bold_green(buffer, E1);
  buffer[F1] = 'B'; set_square_bold_green(buffer, F1);
  buffer[G1] = 'N'; set_square_bold_green(buffer, G1);
  buffer[H1] = 'R'; set_square_bold_green(buffer, H1);

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
