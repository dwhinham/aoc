CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror

DAYS=$(patsubst %.c,%,$(wildcard day*.c))

.PHONY: all clean
all: ${DAYS}

%: %.c
	@echo CC $@
	@$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf ${DAYS}
