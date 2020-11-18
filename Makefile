CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

all: dinowm

config.h:
	cp config.def.h config.h

dinowm: dinowm.c dinowm.h config.h Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< -lX11 $(LDFLAGS)

install: all
	install -Dm755 dinowm $(DESTDIR)$(BINDIR)/dinowm

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/dinowm

clean:
	rm -f dinowm *.o

.PHONY: all install uninstall clean
