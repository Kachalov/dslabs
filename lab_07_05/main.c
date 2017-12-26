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
    hc_pt hc = hc_init(5);
    ho_pt ho = ho_init(5);
    list1_t *l_res = NULL;
    char buf[80];
    char *cpy = NULL;
    char key[2] = "";
    tick_t at = 0;
    tick_t ticks_ho = 0;
    tick_t ticks_hc = 0;
    tick_t ticks_avl = 0;
    tick_t ticks_bst = 0;

    FILE *favlb = fopen("./out/avlb.dot", "w");
    FILE *favle = fopen("./out/avle.dot", "w");
    FILE *fbstb = fopen("./out/bstb.dot", "w");
    FILE *fbste = fopen("./out/bste.dot", "w");

    if (argc < 4)
    {
        printf("Args: filename symbol pre|in|post\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("File could not be read\n");
        return 1;
    }

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
            if ((float)hc->cells / hc->n > 0.5)
                hc_restruct(&hc);

            ho_add(ho, cpy, cpy);
            if ((float)ho->els / ho->n > 0.3)
                ho_restruct(&ho);
        }
        buf[0] = '\0';
    }
    fclose(fp);

    printf("Before: \n");
    printf("AVL: ");
    if (strcmp(argv[3], "pre") == 0) print_nodes_pre(stdout, p);
    else if (strcmp(argv[3], "in") == 0) print_nodes_in(stdout, p);
    else if (strcmp(argv[3], "post") == 0) print_nodes_post(stdout, p);
    printf("\n\n");
    #ifdef DELBST
        printf("BST: ");
        print_bst_in(stdout, bst);
        printf("\n\n");
    #endif
    print_nodes_dot(favlb, p);
    fclose(favlb);
    system("dot -Tpng -o out/avlb.png out/avlb.dot");
    print_bst_dot(fbstb, bst);
    fclose(fbstb);
    system("dot -Tpng -o out/bstb.png out/bstb.dot");
    printf("\nClosed hash (efficiency: %.2f %d/%d):\n",
            (float)hc->els / hc->cells, hc->els, hc->cells);
    hc_print(hc);
    printf("\nOpened hash (efficiency: %.2f %d/%d):\n",
            (float)ho->els / ho->cells, ho->els, ho->cells);
    ho_print(ho);
    printf("\n");

    bst_find_first_letter(bst, argv[2][0], &l_res);
    for_each(it, l_res)
    {
        strcpy(buf, (char *)it->data);

        at = tick();
        hc_del(hc, buf);
        ticks_hc =+ tick() - at - ticks;

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

    printf("After: \n");
    printf("AVL: ");
    if (strcmp(argv[3], "pre") == 0) print_nodes_pre(stdout, p);
    else if (strcmp(argv[3], "in") == 0) print_nodes_in(stdout, p);
    else if (strcmp(argv[3], "post") == 0) print_nodes_post(stdout, p);
    printf("\n\n");
    #ifdef DELBST
        printf("BST: ");
        print_bst_in(stdout, bst);
        printf("\n\n");
    #endif
    print_nodes_dot(favle, p);
    fclose(favle);
    system("dot -Tpng -o out/avle.png out/avle.dot");
    print_bst_dot(fbste, bst);
    fclose(fbste);
    system("dot -Tpng -o out/bste.png out/bste.dot");
    printf("\nClosed hash (efficiency: %.2f %d/%d):\n",
            hc_efficiency(hc), hc->els, hc->cells);
    hc_print(hc);
    printf("\nOpened hash (efficiency: %.2f %d/%d):\n",
            ho_efficiency(ho), ho->els, ho->cells);
    ho_print(ho);
    printf("\n");

    printf("Deleting:\n");
    printf("Ticks opened hash: \033[1;32m%"PRIu64"\033[0m\n", ticks_ho);
    printf("Ticks closed hash: \033[1;32m%"PRIu64"\033[0m\n", ticks_hc);
    printf("Ticks AVL:         \033[1;32m%"PRIu64"\033[0m\n", ticks_avl);
    printf("Ticks BST:         \033[1;32m%"PRIu64"\033[0m\n", ticks_bst);

    return 0;
}
