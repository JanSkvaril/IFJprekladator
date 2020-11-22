# Makefile for IFJ project, Compiler design
# Team 115

CC = gcc
CFLAGS = -g -Wall -pedantic -Wextra
LFLAGS = -lm
OBJ = scanner.o parser.o generator.o sym_table.o main.o error.o

compiler: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

scanner.o: scanner/scanner.c scanner/scanner.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

parser.o: parser/parser.c parser/parser.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

generator.o: generator/generator.c generator/generator.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

sym_table.o: sym_table/sym_table.c sym_table/sym_table.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

error.o: error/error.c error/error.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

./PHONY: clean test
test:
#todo
clean:
	rm -f *.o compiler