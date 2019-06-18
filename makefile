chess.exe : chess.o display.o chessboard_0x88.o
	gcc chess.o display.o chessboard_0x88.o -o chess.exe

chess.o : chess.c
	gcc -c chess.c -o chess.o

display.o : display.c display.h
	gcc -c display.c -o display.o

chessboard.o : chessboard.c chessboard.h
	gcc -c chessboard.c -o chessboard.o

chessboard_0x88.o : chessboard_0x88.c
	gcc -c chessboard_0x88.c -o chessboard_0x88.o

clean :
	rm *.o
	rm *.exe
