CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: similar_lines

similar_lines: similar_lines.o reading.o parsing.o sorting.o printing.o
	$(CC) $(LDFLAGS) -o $@ $^
reading.o: reading.c reading.h structures.h parsing.h
parsing.o: parsing.c parsing.h structures.h
sorting.o: sorting.c sorting.h structures.h
printing.o: printing.c printing.h structures.h sorting.h
similar_lines.o: similar_lines.c structures.h reading.h sorting.h printing.h

clean:
	rm -f *.o similar_lines
