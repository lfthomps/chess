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

  // Testing movement code.  This will eventually be moved out to
  // some nicer automated tests.  
  assert(chessboard_algmove(cb, "e4"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "Nd4"));
  assert(!chessboard_algmove(cb, "Nd2"));
  assert(chessboard_algmove(cb, "Nc3"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "Nh4"));
  assert(chessboard_algmove(cb, "Na4"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "Ng5"));
  assert(!chessboard_algmove(cb, "Nf6"));
  assert(chessboard_algmove(cb, "Nh3"));
  assert(!chessboard_algmove(cb, "Bf2"));
  assert(!chessboard_algmove(cb, "Bg2"));
  assert(chessboard_algmove(cb, "Bc4"));
  display_draw_chessboard(buffer, cb);
  assert(chessboard_algmove(cb, "O-O"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "Kf2"));
  assert(chessboard_algmove(cb, "f3"));
  display_draw_chessboard(buffer, cb);
  assert(chessboard_algmove(cb, "Kf2"));
  display_draw_chessboard(buffer, cb);
  assert(chessboard_algmove(cb, "Ke1"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "Rf3"));
  assert(chessboard_algmove(cb, "Rf2"));
  display_draw_chessboard(buffer, cb);
  assert(chessboard_algmove(cb, "Rf1"));
  display_draw_chessboard(buffer, cb);
  assert(chessboard_algmove(cb, "Rh1"));
  display_draw_chessboard(buffer, cb);
  assert(!chessboard_algmove(cb, "O-O"));

  display_draw_chessboard(buffer, cb);
  printf("\n%s\n", buffer);

  // This is obviously not permanent code - I just want to be able
  // to test things a little more easily.  
  while (true)
  {
      char move_str[32] = {0};

      printf("Enter move (q to quit): ");
      fgets(move_str, 32, stdin);
      if (move_str[0] == 'q')
      {
	  printf("\n");
	  break;
      }
      else if (chessboard_algmove(cb, move_str))
      {
	  display_draw_chessboard(buffer, cb);
	  printf("\n%s\n", buffer);
	  chessboard_switch_current_player(cb);
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
