CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

all: siddwm2

config.h:
	cp config.def.h config.h

siddwm2: siddwm.c siddwm.h config.h Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< -lxcb -lxcb-keysyms $(LDFLAGS)

install: all
	install -Dm755 siddwm2 $(DESTDIR)$(BINDIR)/siddwm2

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/siddwm2

clean:
	rm -f siddwm2 *.o

.PHONY: all install uninstall clean
