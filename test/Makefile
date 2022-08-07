CC=clang
CPP=clang++
CFLAGS=-I$(DIR) -g -Wall
DIR=../src
SRCS=$(DIR)/baby_vm.c $(DIR)/memory.c $(DIR)/hardware.c ../util/hashmap.c
DEPS=$(DIR)/*.h ../util/*.h
OBJS=*.o

test: $(OBJS) test.cpp catch.hh
	$(CPP) $(CFLAGS) -std=c++11 $^

$(OBJS): $(DEPS) $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f -r a.out *.o *.dSYM
