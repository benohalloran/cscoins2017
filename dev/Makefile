CC = g++
CXXFLAGS = -Ofast -march=native -flto -Wall -Wextra -pthread \
		   -fomit-frame-pointer -g3
LDFLAGS = $(CXXFLAGS)
LDLIBS = -lcrypto

.PHONY: all clean

all: perf

perf: perf.o nums_to_str0.o nums_to_str1.o nums_to_str2.o nums_to_str3.o \
	nums_to_str4.o nums_to_str5.o nums_to_str6.o

clean:
	rm -f -- *.ii *.[os] perf *.res *.out
