chess.exe : display.o chessboard.o
	gcc display.o chessboard.o -o chess.exe

display.o : display.c
	gcc -c display.c -o display.o

chessboard.o : chessboard.c chessboard.h
	gcc -c chessboard.c -o chessboard.o

clean :
	rm *.o
	rm *.exe
