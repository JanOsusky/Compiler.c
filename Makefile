##
##   IFJ - Formalni jazyky a prekladace
##   zimni semestr 2022/2023
##
##   Makefile
##
##   @author Vojtech Adamek (xadame44)
##

CC=gcc
CFLAGS= -std=c11 -pedantic -Wall
PROG=dynStr.c main.c parser.c precanal.c precstack.c scanner.c symtable.c symtablestack.c codegen.c
.PHONY: all clean

all: $(PROG)
	$(CC) $(CFLAGS) -o main $(PROG)

clean:
	rm -f *.o
