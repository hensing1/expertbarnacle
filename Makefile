main: main.c
	clang -g -fsanitize=address -static-libasan -lraylib main.c -o main
