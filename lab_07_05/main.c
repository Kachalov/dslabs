#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashc.h"
#include "avl.h"
#include "lib/time.h"

int main(int argc, char **argv)
{
    node_t *p = NULL;
    hc_pt hc = hc_init(8);
    char buf[80];
    char *cpy = NULL;
    char key[2] = "";
    tick_t at = 0;
    tick_t ticks_ho = 0;
    tick_t ticks_hc = 0;
    tick_t ticks_avl = 0;
    tick_t ticks_bst = 0;

    if (argc < 5)
    {
        fprintf(stderr, "Args: filename symbol -|+ pre|in|post\n");
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
        {
            p = insert(p, buf);
            cpy = malloc(strlen(buf) + 1);
            strcpy(cpy, buf);
            strncpy(key, buf, 1);
            hc_add(hc, cpy, cpy);
        }
        buf[0] = '\0';
    }
    fclose(fp);

    if (strcmp("-", argv[3]) == 0)
    {
        if (strcmp(argv[4], "pre") == 0) print_nodes_pre(p);
        else if (strcmp(argv[4], "in") == 0) print_nodes_in(p);
        else if (strcmp(argv[4], "post") == 0) print_nodes_post(p);
        fprintf(stderr, "\n");
        print_nodes_dot(p);
        hc_print(hc);
        fprintf(stderr, "Closed hash efficiency: %.2f\n", (float)hc->els / hc->cells);
    }

    at = tick();
    p = remove_first_letter(p, argv[2][0]);
    ticks_avl = tick() - at;

    if (strcmp("+", argv[3]) == 0)
    {
        if (strcmp(argv[4], "pre") == 0) print_nodes_pre(p);
        else if (strcmp(argv[4], "in") == 0) print_nodes_in(p);
        else if (strcmp(argv[4], "post") == 0) print_nodes_post(p);
        fprintf(stderr, "\n");
        print_nodes_dot(p);
        hc_print(hc);
        fprintf(stderr, "Closed hash efficiency: %.2f\n", (float)hc->els / hc->cells);
    }

    at = tick();
    for_each(it, hc->data[hc_hash(hc, argv[2])])
        if (argv[2][0] == ((hce_pt)it->data)->k[0])
            hc_del(hc, ((hce_pt)it->data)->k);
    ticks_hc = tick() - at;

    fprintf(stderr, "Ticks open hash:  \033[1;32m%"PRIu64"\033[0m\n", ticks_ho);
    fprintf(stderr, "Ticks close hash: \033[1;32m%"PRIu64"\033[0m\n", ticks_hc);
    fprintf(stderr, "Ticks AVL:        \033[1;32m%"PRIu64"\033[0m\n", ticks_avl);
    fprintf(stderr, "Ticks BST:        \033[1;32m%"PRIu64"\033[0m\n", ticks_bst);

    return 0;
}
