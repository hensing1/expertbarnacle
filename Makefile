main: main.c
	clang -fsanitize=address -lraylib main.c -o main
