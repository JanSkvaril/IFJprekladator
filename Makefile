# Makefile for IFJ project, Compiler design
# Team 115

CC = gcc
CFLAGS = -g -Wall -pedantic -Wextra
LDFLAGS=-lm

SRC_DIRS = scanner parser generator error sym_table
SRCS = $(shell find $(SRC_DIRS) -name "*.c")
SRCS += main.c
OBJS = $(addsuffix .o,$(basename $(SRCS)))

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)


.PHONY: clean test deploy zip
test:
	cd tests;\
	python2 testsuite.py --compiler ../compiler;\
	cd ..
#creates folder deploy, copies all source files and updates includes so that everything works wow
deploy:
	./deploy.sh
zip:
	zip -r -j xzavad18.zip deploy
clean:
	find -name "*.o" -delete
	rm -r -f compiler