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

.PHONY: clean

clean:
	rm -f $(DIR)/*.gch *.o baby_vm
