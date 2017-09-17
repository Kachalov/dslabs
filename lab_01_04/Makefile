CC := gcc
CFLAGS := -Wall -Werror -pedantic -std=c99 -DFULLMAN
FILES := main.c
DEST := main.exe

ifeq ($(mode), debug)
	CFLAGS += -g3 -O0 -DDEBUG -fprofile-arcs -ftest-coverage
endif

ifeq ($(mode), release)
	CFLAGS += -DNDEBUG -g0
endif


run:
	$(CC) $(CFLAGS) -o $(DEST) $(FILES)

clean:
	rm -f *.exe *.gcda *.gcno
