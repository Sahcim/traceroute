CC = gcc
CFLAGS = -Wextra -Wall
DEPS = sender.h
OBJ = main.o sender.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

traceroute: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

distclean:
	rm -f *.o traceroute
