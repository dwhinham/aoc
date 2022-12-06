CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror

.PHONY: all
all: day1 day2 day3 day4 day5 day6

day1: day1.o
	$(CC) $(CFLAGS) $< -o $@

day2: day2.o
	$(CC) $(CFLAGS) $< -o $@

day3: day3.o
	$(CC) $(CFLAGS) $< -o $@

day4: day4.o
	$(CC) $(CFLAGS) $< -o $@

day5: day5.o
	$(CC) $(CFLAGS) $< -o $@

day6: day6.o
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o day1 day2 day3 day4 day5 day6
