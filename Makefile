# Makefile for um (Comp 40 Assignment 6)

############## Variables ###############

CC = gcc

IFLAGS  = -I/comp/40/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/lib64 -L/usr/sup/cii40/lib64
LDLIBS  = -l40locality -lcii40-O1 -lm

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um writetests


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

um: um.o memory.o segments.o instructions.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

writetests: umlabwrite.o umlab.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f um writetests *.o
	rm -f writetests *.0
	rm -f writetests *.1
