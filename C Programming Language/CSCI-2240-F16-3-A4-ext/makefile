CFLAGS := -Wall -ansi -pedantic -c -g 

ALL: wordformat.out

wordformat.o : wordformat.c
	gcc $(CFLAGS) wordformat.c

wordformat.out : wordformat.o
	gcc -g -o wordformat.out wordformat.o

clean:
	rm -f *.o
	rm -f *.out
