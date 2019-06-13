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
      printf("Failed to allocate buffer\n");
      return -1;
    }

  chessboard cb = {};
  chessboard_initialize_board(&cb);

  int buffer_index = 0;
  buffer_index = fill_board(buffer, buffer_index);

  draw_chessboard(buffer, &cb);

  printf("\n");
  printf("%s\n", buffer);

  free(buffer);

  return 0;
}
