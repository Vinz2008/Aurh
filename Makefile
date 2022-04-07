CC=gcc
CFLAGS=-lcurl

all:
	$(CC) main.c $(CFLAGS) -o aurh

install:
	cp aurh /usr/bin/aurh

uninstall:
	rm /usr/bin/aurh