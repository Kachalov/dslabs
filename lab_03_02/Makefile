CC := gcc
CFLAGS := -Wall -Werror -pedantic -std=c11
OBJS := lib/list.o

ifeq ($(mode), debug)
	CFLAGS += -g3 -O0 -DDEBUG -fprofile-arcs -ftest-coverage
endif

ifeq ($(mode), release)
	CFLAGS += -g0 -O3 -DNDEBUG
endif

ifeq ($(MAKECMDGOALS), main.so)
	CFLAGS += -fpic
endif

ifeq ($(sort_alg), qsort)
	CFLAGS += -DSORT_QSORT
endif

main.exe: $(OBJS) main.o
	$(CC) $(CFLAGS) $^ -o $@

test.exe: $(OBJS) test.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

main.so: $(OBJS)
	$(CC) $(CFLAGS) -shared $^ -o $@

clean:
	rm -f *.exe *.gcda *.gcno *.o *.so
