#CFLAGS+=-Wall -W -ggdb3 -std=c17 -O0
CFLAGS+=-Wall -O3 -ljemalloc -lstdc++


.PHONY: all clean clean-rabin

all: rabin-cdc

rabin.o: rabin.c rabin.h

rabin-cdc: main.o rabin.o
	$(CC) $(CFLAGS) -o $@ $^

clean: clean-rabin

clean-rabin:
	rm -f rabin.o main.o rabin-cdc
