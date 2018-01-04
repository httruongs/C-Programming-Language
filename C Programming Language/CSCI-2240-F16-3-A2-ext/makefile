CFLAGS := -Wall -ansi -pedantic -c -g 

ALL: wordsearch.out

wordsearch.o : wordsearch.c
	gcc $(CFLAGS) wordsearch.c

wordsearch.out : wordsearch.o
	gcc -g -o wordsearch.out wordsearch.o

clean:
	rm -f *.o
	rm -f *.out
