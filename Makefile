.DEFAULT_GOAL: snake

snake:
	cc -g -Wall -O0 -o snake nicolaiSnake/main.c -lncurses

clean:
	rm snake
