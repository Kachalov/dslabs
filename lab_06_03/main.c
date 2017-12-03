#include <stdlib.h>
#include <stdio.h>

#include "avl.h"

int main(void)
{
    node_t *p = NULL;
    char buf[80];

    FILE *fp = fopen("test.txt", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%79s", buf);
        p = insert(p, buf);
    }

    print_nodes_dot(p);

    return 0;
}