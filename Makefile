main: main.c
	@mkdir -p build
	clang -g -fsanitize=address -lraylib main.c -o build/main

ui: ui.c
	@mkdir -p build
	clang -g -fsanitize=address -lraylib ui.c -o build/ui
