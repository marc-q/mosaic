CC = gcc
OBJECTS = libbmp.c mosaic.c
LIBS =
CFLAGS = -Wall -O2
BINDIR = $(DESTDIR)/usr/bin
NAME = mosaic

mosaic: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm $(NAME)
