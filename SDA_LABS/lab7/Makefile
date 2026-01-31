CC=gcc
CFLAGS = -Wall -Wextra -g


final :
	gcc $(CFLAGS) main2.c -o run
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="valgrind.out" ./run

clean : 
	rm run valgrind.out
