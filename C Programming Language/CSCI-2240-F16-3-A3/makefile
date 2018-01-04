CFLAGS := -Wall -ansi -pedantic -c -g 

ALL: Virtual_Machine.out

Virtual_Machine.o : Virtual_Machine.c
	gcc $(CFLAGS) Virtual_Machine.c

Virtual_Machine.out : Virtual_Machine.o
	gcc -g -o Virtual_Machine.out Virtual_Machine.o

clean:
	rm -f *.o
	rm -f *.out
