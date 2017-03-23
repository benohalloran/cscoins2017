CC = g++
CPPFLAGS = -Ofast -g3 -march=native -flto -Wall -Wextra -std=gnu++14 -pthread \
		   -save-temps -I ../include -DTESTING
LDFLAGS = $(CPPFLAGS) -lcrypto

.PHONY: all clean

all: test

test: test.o solver.o parser.o

clean:
	rm -f -- test *.ii *.[os] *.res *.out
