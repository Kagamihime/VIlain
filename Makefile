CC = gcc
CFLAGS = -Wall -std=c11
COMPILE = $(CC) $(CFLAGS) -c

vilain: ./build/main.o ./build/cursor.o ./build/buffer.o ./build/io_text.o ./build/ui.o ./build/settings.o
	$(CC) $^  -o ./build/$@ -lncurses -lpthread

build/buffer.o: ./src/buffer.c ./include/buffer.h
	$(COMPILE)	$< -o $@

build/cursor.o: ./src/cursor.c ./include/cursor.h
	$(COMPILE)	$< -o $@

build/io_text.o: ./src/io_text.c ./include/io_text.h
	$(COMPILE)	$< -o $@

build/settings.o: ./src/settings.c ./include/settings.h
	$(COMPILE)	$< -o $@

build/ui.o: ./src/ui.c ./include/ui.h
	$(COMPILE)	$<	-o $@

build/main.o: ./src/main.c
	$(COMPILE)	$< -o $@

clean:
	@rm -f ./build/*.o
	@echo "Cleared"
