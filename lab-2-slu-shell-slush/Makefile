CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean

all: slush

slush: slush.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f slush
