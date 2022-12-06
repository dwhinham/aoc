CC=gcc
CFLAGS=-Iinclude -Wall -Wextra -Wpedantic -Werror

DAYS=$(patsubst src/%.c,%,$(wildcard src/day*.c))

.PHONY: all clean
all: ${DAYS}

%: src/%.c
	@echo CC $@
	@$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf ${DAYS}
