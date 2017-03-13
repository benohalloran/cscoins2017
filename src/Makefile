CC = g++
CPPFLAGS = -O3 -g3 -march=native -flto -Wall -Wextra -pthread -std=gnu++14 \
	-pthread -save-temps -I ../include -DTESTING
LDFLAGS = $(CPPFLAGS) -lcrypto

.PHONY: all clean

all: test

test: test.o solver.o sha256.o

clean:
	rm -f -- test *.ii *.[os] *.res *.out
