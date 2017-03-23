CC = g++
CXXFLAGS = -Ofast -march=native -flto -Wall -Wextra -pthread \
		   -fomit-frame-pointer -g3 -I ../include/
LDFLAGS = $(CXXFLAGS)
LDLIBS = -lcrypto

.PHONY: all clean

all: perf test

perf: perf.o nums_to_str0.o nums_to_str1.o nums_to_str2.o nums_to_str3.o \
	nums_to_str4.o nums_to_str5.o nums_to_str6.o nums_to_str7.o \
	nums_to_str8.o sorting0.o sorting1.o sorting2.o sorting3.o sorting4.o \
	sorting5.o sorting6.o sorting7.o

test: test.o nums_to_str0.o nums_to_str1.o nums_to_str2.o nums_to_str3.o \
	nums_to_str4.o nums_to_str5.o nums_to_str6.o nums_to_str7.o \
	nums_to_str8.o parser.o sorting0.o sorting1.o sorting2.o sorting3.o \
	sorting4.o sorting5.o sorting6.o sorting7.o

clean:
	rm -f -- *.ii *.[os] perf test *.res *.out
