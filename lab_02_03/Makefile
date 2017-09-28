CC := gcc
# mode=debug - for debug
CFLAGS := -Wall -Werror -pedantic -std=c11
FILES := main.c data.c io.c
DEST := main.exe
DEST_SO := main.so

ifeq ($(mode), debug)
	CFLAGS += -g3 -O0 -DDEBUG -fprofile-arcs -ftest-coverage
endif

ifeq ($(mode), release)
	CFLAGS += -DNDEBUG -g0
endif


main.exe:
	$(CC) $(CFLAGS) -o $(DEST) $(FILES)

main.so:
	$(CC) -shared -fpic $(CFLAGS) -o $(DEST_SO) $(FILES)

clean:
	rm -f *.exe *.gcda *.gcno
