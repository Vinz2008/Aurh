all:
	gcc main.c -o aurh

install:
	cp aurh.sh /usr/bin/aurh

uninstall:
	rm /usr/bin/aurh