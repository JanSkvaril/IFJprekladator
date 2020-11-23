#! /bin/bash

mkdir deploy
declare -a dirs=("error" "generator" "parser" "scanner" "sym_table")
declare -a files=("main.c
")
for i in ${dirs[@]}; do
	files+=$(find $i -type f -name "*.c" -o -name "*.h")
	files+="
	"
done
for i in ${files[@]}; do
	cp $i deploy
done

files=$(find deploy -type f)

for i in ${files[@]}; do 
	sed  's/^#include ".*\//#include "/' $i > tmp
	cat tmp > $i
done
rm tmp

echo "
CC = gcc
CFLAGS = -g -Wall -pedantic -Wextra
LDFLAGS=-lm

SRC_DIRS = scanner parser generator error sym_table
SRCS = \$(shell find . -name \"*.c\")
OBJS = \$(addsuffix .o,\$(basename \$(SRCS)))

compiler: \$(OBJS)
	\$(CC) \$(CFLAGS) -o \$@ \$(OBJS) \$(LDFLAGS)

.PHONY: clean

clean:
	rm -r *.o" > deploy/Makefile