#include "display.h"

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
const int square_width = 7;
const int board_width = 58;
// The array display_squares converts the standard square enum (A8=0,
//..., H1=63) to buffer indices.  For instance, display_squares[A8]
//will give the index of the character at the center of the A8 square
//in the buffer.
const int display_squares[64] =
{131, 151, 171, 191, 211, 231, 251, 271,
 412, 432, 452, 472, 492, 512, 532, 552,
 693, 713, 733, 753, 773, 793, 813, 833,
 974, 994, 1014, 1034, 1054, 1074, 1094, 1114,
 1255, 1275, 1295, 1315, 1335, 1355, 1375, 1395,
 1536, 1556, 1576, 1596, 1616, 1636, 1656, 1676,
 1817, 1837, 1857, 1877, 1897, 1917, 1937, 1957,
 2098, 2118, 2138, 2158, 2178, 2198, 2218, 2238};

int _fill_line(char* buffer, int buffer_index);
int _fill_interior_line(char* buffer, int buffer_index, bool light_square);
int _fill_colored_interior_line(char* buffer, int buffer_index, bool light_square);
int _fill_rank(char* buffer, int buffer_index, bool light_square);
void _set_square_bold_red(char* buffer, int ind);
void _set_square_bold_green(char* buffer, int ind);

int _fill_line(char* buffer, int buffer_index)
{
  int stop_index = buffer_index + board_width;
  for ( ; buffer_index < stop_index; buffer_index++)
    {
      buffer[buffer_index] = '#';
    }
  buffer[buffer_index++] = '\n';

  return buffer_index;
}

int _fill_interior_line(char* buffer, int buffer_index, bool light_square)
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

int _fill_colored_interior_line(char* buffer, int buffer_index, bool light_square)
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

int _fill_rank(char* buffer, int buffer_index, bool light_square)
{
  buffer_index = _fill_interior_line(buffer, buffer_index, light_square);
  buffer_index = _fill_colored_interior_line(buffer, buffer_index, light_square);
  buffer_index = _fill_interior_line(buffer, buffer_index, light_square);

  return buffer_index;
}

int display_fill_board(char* buffer, int buffer_index)
{
  bool light_square = true;
  buffer_index = _fill_line(buffer, buffer_index);
  for (int rank = 0; rank < 8; rank++)
    {
      buffer_index = _fill_rank(buffer, buffer_index, light_square);
      light_square = !(light_square);
    }
  buffer_index = _fill_line(buffer, buffer_index);

  return buffer_index;
}

void _set_square_bold_red(char* buffer, int ind)
{
  // Change to bold escape code
  buffer[ind-7] = '1';
  // Change to red escape code
  buffer[ind-2] = '1';
}

void _set_square_bold_green(char* buffer, int ind)
{
  // Change to bold escape code
  buffer[ind-7] = '1';
  // Change to green escape code
  buffer[ind-2] = '2';
}

void display_draw_chessboard(char* buffer, chessboard* cb)
{
    for (enum chessboard_square square = A8; square < CHESSBOARD_MAX_SQUARE; square++)
    {
	enum chessboard_color color = chessboard_get_color(cb, square);
	enum chessboard_piecetype type = chessboard_get_piecetype(cb, square);
	int ind = display_squares[square];
	if (type != EMPTY)
	{
	    if (color == WHITE)
	    {
		_set_square_bold_green(buffer, ind);
	    }
	    else if (color == BLACK)
	    {
		_set_square_bold_red(buffer, ind);
	    }
	}
	switch (type)
	{
	case PAWN:
	    buffer[ind] = 'P';
	    break;
	case KNIGHT:
	    buffer[ind] = 'N';
	    break;
	case BISHOP:
	    buffer[ind] = 'B';
	    break;
	case QUEEN:
	    buffer[ind] = 'Q';
	    break;
	case KING:
	    buffer[ind] = 'K';
	    break;
	case ROOK:
	    buffer[ind] = 'R';
	}
    }
}
