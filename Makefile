

CC=gcc
CFLAGS=-Wall -O3
CFLAGS += $(shell pkg-config --cflags libcurl)
LDFLAGS += $(shell pkg-config --libs libcurl)

all:
	$(CC) main.c $(CFLAGS) -o aurh $(LDFLAGS)

install:
	cp aurh /usr/bin/aurh

uninstall:
	rm /usr/bin/aurh
