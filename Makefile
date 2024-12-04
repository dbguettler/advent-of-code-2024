CFLAGS = -Wall -std=c99
CC = gcc

all: day1 day2 day3 day4

day4: day4.o aoc_input.o list.o strings.o
	$(CC) day4.o aoc_input.o list.o strings.o -o day4
	
day4.o: day4.c list.h strings.h aoc_input.h

day3: day3.o aoc_input.o list.o strings.o
	$(CC) day3.o aoc_input.o list.o strings.o -o day3
	
day3.o: day3.c list.h strings.h aoc_input.h

day2: day2.o aoc_input.o list.o strings.o
	$(CC) day2.o aoc_input.o list.o strings.o -o day2
	
day2.o: day2.c list.h strings.h aoc_input.h

day1: day1.o aoc_input.o list.o strings.o
	$(CC) day1.o aoc_input.o list.o strings.o -o day1
	
day1.o: day1.c list.h strings.h aoc_input.h

list.o: list.c list.h

strings.o: strings.c strings.h

aoc_input.o: aoc_input.c aoc_input.h

clean:
	rm -f *.o
	rm -f day1 day2 day3 day4