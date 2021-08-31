CC=clang
CFLAGS=-g -I.$(DIR)
DIR=./src
# DEPS = memory.c hardware.c util.c
OBJ = baby_vm.o

%.o: $(DIR)/%.c #$(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

baby_vm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~ baby_vm