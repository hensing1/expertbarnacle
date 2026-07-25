main: main.c
	clang -g -fsanitize=address -lraylib main.c -o build/main

ui: ui.c
	clang -g -fsanitize=address -lraylib ui.c -o build/ui
