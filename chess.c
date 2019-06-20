#include <stdio.h>
#include <ctype.h>
#include "display.h"
#include "chessboard_api.h"

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
      printf("DEBUG: Failed to allocate buffer\n");
      return -1;
    }

  chessboard* cb = chessboard_allocate();
  if (!cb)
  {
      printf("DEBUG: Failed to allocate board\n");
      return -2;
  }
  chessboard_initialize_board(cb);

  int buffer_index = 0;
  buffer_index = display_fill_board(buffer, buffer_index);

  display_draw_chessboard(buffer, cb);

  printf("\n");
  printf("%s\n", buffer);

  assert(!chessboard_move(cb, E2, E4));
  assert(!chessboard_move(cb, B1, D4));
  assert(!chessboard_move(cb, B1, D2));
  assert(chessboard_move(cb, B1, C3));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_move(cb, C3, H4));
  assert(chessboard_move(cb, C3, A4));
  assert(!chessboard_move(cb, A4, G5));
  assert(!chessboard_move(cb, G8, F6));
  assert(!chessboard_move(cb, E5, G4));
  assert(chessboard_move(cb, G1, H3));

  display_draw_chessboard(buffer, cb);
  printf("\n%s\n", buffer);

  // This is obviously not permanent code - I just want to be able
  // to test things a little more easily.  
  while (true)
  {
      chessboard_square from = CHESSBOARD_MAX_SQUARE;
      chessboard_square to = CHESSBOARD_MAX_SQUARE;
      char input_buffer[4] = {};
      unsigned char rank = '\0';
      unsigned char file = '\0';
      bool valid_square = true;
      
      printf("Enter from square: ");
      if (input_buffer[0] == 'q')
      {
	  printf("\n");
	  break;
      }
      fgets(input_buffer, 4, stdin);
      file = tolower(input_buffer[0]);
      rank = input_buffer[1];
      if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8')
      {
	  from = (7 - (int)(rank - '1')) * 8 + (int)(file - 'a');
      }
      else
      {
	  valid_square = false;
      }
      printf("Enter to square: ");
      if (input_buffer[0] == 'q')
      {
	  printf("\n");
	  break;
      }
      fgets(input_buffer, 4, stdin);
            file = tolower(input_buffer[0]);
      rank = input_buffer[1];
      if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8')
      {
	  to = (7 - (int)(rank - '1')) * 8 + (int)(file - 'a');
      }
      else
      {
	  valid_square = false;
      }

      if (valid_square && chessboard_move(cb, from, to))
      {
	  display_draw_chessboard(buffer, cb);
	  printf("\n%s\n", buffer);
      }
      else
      {
	  printf("Illegal move\n");
      }
      
  }

  chessboard_free(cb);
  free(buffer);

  return 0;
}
