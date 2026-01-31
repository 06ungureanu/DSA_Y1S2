CC=gcc
CFLAGS= -Wall -Wextra -g -lm  

all : final

final :
	gcc $(CFLAGS) main.c -o run
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="valgrind.out" ./run

clean :
	rm run valgrind.out*
