chess.exe : chess.o display.o chessboard_0x88.o move_0x88.o algmove_0x88.o
	gcc chess.o display.o chessboard_0x88.o move_0x88.o algmove_0x88.o -o chess.exe

chess.o : chess.c
	gcc -c chess.c -o chess.o

display.o : display.c display.h
	gcc -c display.c -o display.o

algmove_0x88.o : algmove_0x88.c algmove_0x88.h
	gcc -c algmove_0x88.c -o algmove_0x88.o

move_0x88.o : move_0x88.c move_0x88.h
	gcc -c move_0x88.c -o move_0x88.o

chessboard_0x88.o : chessboard_0x88.c chessboard_0x88.h
	gcc -c chessboard_0x88.c -o chessboard_0x88.o

clean :
	rm *.o
	rm *.exe
