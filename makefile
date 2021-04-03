CC = gcc
CFLAGS = -std=gnu99 -Wextra -Wall
DEPS = sender.h reciver.h
OBJ = main.o sender.o reciver.o

traceroute: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

distclean:
	rm -f *.o traceroute
