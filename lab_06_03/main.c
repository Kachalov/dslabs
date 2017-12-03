#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl.h"

int main(int argc, char **argv)
{
    node_t *p = NULL;
    char buf[80];

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
    p = remove_first_letter(p, argv[2][0]);
    if (strcmp("+", argv[3]) == 0)
        print_nodes_dot(p);

    return 0;
}