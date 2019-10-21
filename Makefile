# Shikhar Sinha, Fall 2019
#
PROG1 = m2rtu
PROG2 = rtu2m
OBJ1 = m2rtu.c
OBJ2 = rtu2m.c
HAM = -lhammer `pkg-config --libs glib-2.0`
CC = gcc
MAKE = make


.PHONY: all clean
all:
	$(CC) -o $(PROG1) $(OBJ1) $(HAM)
	$(CC) -o $(PROG2) $(OBJ2) $(HAM)

clean:
	rm -f *~ *.o *.dYSM
	rm -f rtu2m m2rtu
