CC = gcc
CFLAGS = -Wall -std=c11
LDFLAGS = $(CFLAGS)
LDLIBS = -Wall $(shell pkg-config --libs check)

all: check_vilain
check_vilain: check_vilain.o ../build/buffer.o ../build/cursor.o ../build/io_text.o ../build/settings.o ../build/ui.o

test: check_vilain
	./$<

clean:
	rm -f check_vilain *.o
