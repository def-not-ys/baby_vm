CC=clang
CFLAGS=-I$(DIR) -g
DIR=./src
SRCS=$(DIR)/*.c
DEPS=$(DIR)/*.h
OBJS=*.o

baby_vm: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(DEPS) $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

hashmap: util/hashmap.c
	$(CC) $(CFLAG) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o baby_vm hashmap