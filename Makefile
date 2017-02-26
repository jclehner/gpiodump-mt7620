ifneq ($(CROSS_COMPILE),)
	CC = $(CROSS_COMPILE)cc
	STRIP = $(CROSS_COMPILE)strip
else
	CC ?= cc
	STRIP ?= strip
endif

LDFLAGS ?= -static

gpiodump: gpiodump.c
	$(CC) gpiodump.c -o gpiodump -Wall $(LDFLAGS)
	$(STRIP) gpiodump

clean:
	rm -f gpiodump gpiodump-mipsel-static

release: clean
	make gpiodump CROSS_COMPILE=/opt/buildroot-gcc342/bin/mipsel-linux-
	mv gpiodump gpiodump-mipsel-static
