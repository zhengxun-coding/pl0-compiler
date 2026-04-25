CC = gcc
CFLAGS = -Wall

pl0: src/main.c src/lexer.c src/parser.c src/symbol.c src/codegen.c src/vm.c src/platform.c src/set.c
	$(CC) $(CFLAGS) -o pl0 src/*.c

clean:
	rm -f pl0
	rm -f *.tmp

.PHONY: clean
