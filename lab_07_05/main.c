#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "hashc.h"
#include "hasho.h"
#include "avl.h"
#include "bst.h"
#include "lib/list.h"
#include "lib/time.h"

int main(int argc, char **argv)
{
    node_t *p = NULL;
    bst_pt bst = NULL;
    hc_pt hc = hc_init(131);
    ho_pt ho = ho_init(131);
    list1_t *l_res = NULL;
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

    init_hash_pow(100);

    while (!feof(fp))
    {
        fscanf(fp, "%79s", buf);
        if (strcmp(buf, ""))
        {
            p = insert(p, buf);
            bst = bst_add(bst, buf, buf);
            cpy = malloc(strlen(buf) + 1);
            strcpy(cpy, buf);
            strncpy(key, buf, 1);
            hc_add(hc, cpy, cpy);
            ho_add(ho, cpy, cpy);
        }
        buf[0] = '\0';
    }
    fclose(fp);

    if (strcmp("-", argv[3]) == 0)
    {
        fprintf(stderr, "AVL: ");
        if (strcmp(argv[4], "pre") == 0) print_nodes_pre(p);
        else if (strcmp(argv[4], "in") == 0) print_nodes_in(p);
        else if (strcmp(argv[4], "post") == 0) print_nodes_post(p);
        fprintf(stderr, "\n\n");
        #ifdef DELBST
            fprintf(stderr, "BST: ");
            print_bst_in(bst);
            fprintf(stderr, "\n\n");
        #endif
        print_nodes_dot(p);
        fprintf(stderr, "\nClosed hash (efficiency: %.2f %d/%d):\n",
                (float)hc->els / hc->cells, hc->els, hc->cells);
        hc_print(hc);
        fprintf(stderr, "\nOpened hash (efficiency: %.2f %d/%d):\n",
                (float)ho->els / ho->cells, ho->els, ho->cells);
        ho_print(ho);
        fprintf(stderr, "\n");
    }

    bst_find_first_letter(bst, argv[2][0], &l_res);
    for_each(it, l_res)
    {
        strcpy(buf, (char *)it->data);

        at = tick();
        hc_del(hc, buf);
        ticks_hc =+ tick() - at;

        at = tick();
        ho_del(ho, buf);
        ticks_ho =+ tick() - at;

        pop_front(&l_res);
    }

    at = tick();
    p = remove_first_letter(p, argv[2][0]);
    ticks_avl = tick() - at;

    #ifdef DELBST
        at = tick();
        bst = bst_remove_first_letter(bst, argv[2][0]);
        ticks_bst = tick() - at;
    #endif

    if (strcmp("+", argv[3]) == 0)
    {
        fprintf(stderr, "AVL: ");
        if (strcmp(argv[4], "pre") == 0) print_nodes_pre(p);
        else if (strcmp(argv[4], "in") == 0) print_nodes_in(p);
        else if (strcmp(argv[4], "post") == 0) print_nodes_post(p);
        fprintf(stderr, "\n\n");
        #ifdef DELBST
            fprintf(stderr, "BST: ");
            print_bst_in(bst);
            fprintf(stderr, "\n\n");
        #endif
        print_nodes_dot(p);
        fprintf(stderr, "\nClosed hash (efficiency: %.2f %d/%d):\n",
                (float)hc->els / hc->cells, hc->els, hc->cells);
        hc_print(hc);
        fprintf(stderr, "\nOpened hash (efficiency: %.2f %d/%d):\n",
                (float)ho->els / ho->cells, ho->els, ho->cells);
        ho_print(ho);
        fprintf(stderr, "\n");
    }

    fprintf(stderr, "Deleting:\n");
    fprintf(stderr, "Ticks opened hash: \033[1;32m%"PRIu64"\033[0m\n", ticks_ho);
    fprintf(stderr, "Ticks closed hash: \033[1;32m%"PRIu64"\033[0m\n", ticks_hc);
    fprintf(stderr, "Ticks AVL:         \033[1;32m%"PRIu64"\033[0m\n", ticks_avl);
    fprintf(stderr, "Ticks BST:         \033[1;32m%"PRIu64"\033[0m\n", ticks_bst);

    return 0;
}
