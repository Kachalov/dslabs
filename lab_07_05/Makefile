CC := gcc
CFLAGS := -Wall -Werror -pedantic -std=gnu11
OBJS := lib/time.o lib/list.o avl.o hasho.o hashc.o

ifeq ($(mode), debug)
	CFLAGS += -g3 -O0 -DDEBUG -fprofile-arcs -ftest-coverage
endif

ifeq ($(mode), release)
	CFLAGS += -g0 -O3 -DNDEBUG
endif

ifeq ($(MAKECMDGOALS), main.so)
	CFLAGS += -fpic
endif

main.run: $(OBJS) main.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

main.so: $(OBJS)
	$(CC) $(CFLAGS) -shared $^ -o $@

memcheck: main.run
	valgrind --leak-check=yes ./main.run 1>/dev/null

clean:
	rm -f *.exe *.gcda *.gcno *.o *.so
