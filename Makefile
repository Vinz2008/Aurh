

CC=gcc
CFLAGS=-lcurl
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)




all:
	$(CC) main.c $(CFLAGS) -o aurh $(LDFLAGS)

install:
	cp aurh /usr/bin/aurh

uninstall:
	rm /usr/bin/aurh
