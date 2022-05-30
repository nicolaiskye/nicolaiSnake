.DEFAULT_GOAL: snake

snake:
	cc -o snake nicolaiSnake/main.c -lncurses

clean:
	rm snake
