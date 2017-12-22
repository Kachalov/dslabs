CC := gcc
CFLAGS := -Wall -Werror -pedantic -std=gnu11
OBJS := lib/list.o lib/mtrx.o lib/mtrx_apply.o lib/time.o graph.o

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

graph: main.run
	echo -e '2\n' | ./main.run in1.txt 3>&1 1>/dev/null 2>&3 | dot -T png -o res.png


clean:
	rm -f *.exe *.gcda *.gcno *.o *.so
