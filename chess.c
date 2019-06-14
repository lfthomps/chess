#include <stdio.h>
#include "display.h"
#include "chessboard.h"

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

  _move_unchecked(cb, E2, E4);
  display_draw_chessboard(buffer, cb);
  printf("\n%s\n", buffer);

  chessboard_free(cb);
  free(buffer);

  return 0;
}
