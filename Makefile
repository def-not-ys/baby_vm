CC=clang
CFLAGS=-I$(DIR) -Wall -g
DIR=./src
SRCS=$(DIR)/baby_vm.c $(DIR)/memory.c $(DIR)/hardware.c $(DIR)/main.c ./util/hashmap.c
DEPS=$(DIR)/*.h ./util/*.h
OBJS=*.o

baby_vm: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(DEPS) $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

.PHONY: clean

clean:
	rm -f -r *.o baby_vm *.dSYM
