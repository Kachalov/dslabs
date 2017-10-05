CC := gcc
# mode=debug - for debug
# sort=index
CFLAGS := -Wall -Werror -pedantic -std=c11
OBJS := bits.o data.o io.o time.o

ifeq ($(mode), debug)
	CFLAGS += -g3 -O0 -DDEBUG -fprofile-arcs -ftest-coverage
endif

ifeq ($(mode), release)
	CFLAGS += -DNDEBUG
endif

ifeq ($(sort), index)
	CFLAGS += -DSORT_NDX
endif

main.exe: $(OBJS) main.o
	$(CC) $(CFLAGS) $^ -o $@

test.exe: $(OBJS) test.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

main.so:
	$(CC) -shared -fpic $(CFLAGS) $^ -o $@

clean:
	rm -f *.exe *.gcda *.gcno *.o
