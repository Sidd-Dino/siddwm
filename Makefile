CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

all: siddwm

config.h:
	cp config.def.h config.h

siddwm: siddwm.c siddwm.h config.h Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< -lX11 $(LDFLAGS)

install: all
	install -Dm755 siddwm $(DESTDIR)$(BINDIR)/siddwm

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/siddwm

clean:
	rm -f siddwm *.o

.PHONY: all install uninstall clean
