CC=gcc
CFLAGS= -Wall -Wextra -g 

all : final

final :
	gcc $(CFLAGS) metoda2.c -o run
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="valgrind.out" ./run

clean :
	rm valgrind.out run
