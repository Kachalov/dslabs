#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl.h"
#include "lib/time.h"

int main(int argc, char **argv)
{
    node_t *p = NULL;
    char buf[80];
    tick_t at = 0;
    tick_t ticks_tree = 0;
    tick_t ticks_file = 0;

    if (argc < 4)
    {
        fprintf(stderr, "Args: filename symbol -|+\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "File could not be read\n");
        return 1;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%79s", buf);
        if (strcmp(buf, ""))
            p = insert(p, buf);
        buf[0] = '\0';
    }

    if (strcmp("-", argv[3]) == 0)
        print_nodes_dot(p);

    at = tick();
    p = remove_first_letter(p, argv[2][0]);
    ticks_tree = tick() - at;

    FILE *fout = fopen("out.txt", "w");
    fseek(fp, 0, SEEK_SET);
    at = tick();
    while (!feof(fp))
    {
        fscanf(fp, "%79s", buf);
        if (strcmp(buf, ""))
            if (buf[0] == argv[2][0])
                fprintf(fout, "%s\n", buf);

        buf[0] = '\0';
    }
    ticks_file = tick() - at;

    if (strcmp("+", argv[3]) == 0)
        print_nodes_dot(p);

    fprintf(stderr, "\nTicks  tree: \033[1;31m%"PRIu64"\033[0m\n", ticks_tree);
    fprintf(stderr, "Ticks file: \033[1;32m%"PRIu64"\033[0m\n", ticks_file);

    if (fp)
        fclose(fp);

    return 0;
}
