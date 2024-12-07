CFLAGS = -Wall -std=c99
CC = gcc

all: day1 day2 day3 day4 day5 day6 day7

day7: day7.o aoc_input.o list.o strings.o
	$(CC) day7.o aoc_input.o list.o strings.o -lm -o day7
	
day7.o: day7.c list.h strings.h aoc_input.h

day6: day6.o aoc_input.o list.o strings.o
	$(CC) day6.o aoc_input.o list.o strings.o -o day6
	
day6.o: day6.c list.h strings.h aoc_input.h

day5: day5.o aoc_input.o list.o strings.o hashmap.o
	$(CC) day5.o aoc_input.o list.o strings.o hashmap.o -o day5
	
day5.o: day5.c list.h strings.h aoc_input.h hashmap.h

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

hashmap.o: hashmap.c hashmap.h

list.o: list.c list.h

strings.o: strings.c strings.h

aoc_input.o: aoc_input.c aoc_input.h

.PHONY: clean
clean:
	rm -f *.o
	rm -f day1 day2 day3 day4 day5 day6 day7
	rm -f vgcore.*