#include <stdlib.h>
#include <stdio.h>

#include "avl.h"

int main(void)
{
    node_t *p = node_init('c');
    p = insert(p, 'a');
    p = insert(p, 'e');
    p = insert(p, 'b');
    p = insert(p, 'd');
    print_nodes(p);
    printf("\n");

    return 0;
}