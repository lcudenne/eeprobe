CC=mpicc
CFLAGS=-g -fPIC -Wall -Werror
DEPS = eeprobe.h
OBJ = eeprobe.o eetest.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

eetest: $(OBJ)
	$(CC) -o $@ $^

