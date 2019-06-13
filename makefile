chess.exe : chess.o display.o chessboard.o
	gcc chess.o display.o chessboard.o -o chess.exe

chess.o : chess.c
	gcc -c chess.c -o chess.o

display.o : display.c display.h
	gcc -c display.c -o display.o

chessboard.o : chessboard.c chessboard.h
	gcc -c chessboard.c -o chessboard.o

clean :
	rm *.o
	rm *.exe
