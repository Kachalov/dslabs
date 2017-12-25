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
#include "lib/debug.h"

int main(int argc, char **argv)
{
    node_t *p = NULL;
    bst_pt bst = NULL;
    hc_pt hc = hc_init(5);
    ho_pt ho = ho_init(5);
    char buf[80];
    char *cpy = NULL;
    char key[2] = "";
    tick_t at = 0;
    tick_t ticks_ho = 0;
    tick_t ticks_hc = 0;
    tick_t ticks_avl = 0;
    tick_t ticks_bst = 0;

    int ac = 0;
    int cmp_ho = 0;
    int cmp_hc = 0;
    int cmp_avl = 0;
    int cmp_bst = 0;

    int code = 0;

    FILE *fbst = fopen("./out/bst.dot", "w");
    FILE *favl = fopen("./out/avl.dot", "w");

    if (argc < 2)
    {
        printf("Args: filename\n");
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
            {
                DPRINT("Closed hash (efficiency: %.2f %d/%d)",
                        hc_efficiency(hc), hc->els, hc->cells);
                hc_restruct(&hc);
            }

            ho_add(ho, cpy, cpy);
            if ((float)ho->els / ho->n > 0.3)
            {
                DPRINT("Opened hash (efficiency: %.2f %d/%d)",
                        ho_efficiency(ho), ho->els, ho->cells);
                ho_restruct(&ho);
            }
        }
        buf[0] = '\0';
    }
    fclose(fp);

    print_nodes_dot(favl, p);
    fclose(favl);
    system("dot -Tpng -o out/avl.png out/avl.dot");
    favl = fopen("./out/avl.dot", "w");
    print_bst_dot(fbst, bst);
    fclose(fbst);
    system("dot -Tpng -o out/bst.png out/bst.dot");
    fbst = fopen("./out/bst.dot", "w");

    do
    {
        printf("1 - add word\n2 - del word\n3 - find word\n4 - show\n0 - exit\n");
        if (scanf("%d", &code) != 1)
            code = 0;

        switch (code)
        {
            case 1:
                printf("Word: ");
                scanf("%79s", buf);
                if (strcmp(buf, ""))
                {
                    ac = avl_cmps;
                    at = tick();
                    p = insert(p, buf);
                    ticks_avl = tick() - at;
                    cmp_avl = avl_cmps - ac;

                    ac = bst_cmps;
                    at = tick();
                    bst = bst_add(bst, buf, buf);
                    ticks_bst = tick() - at;
                    cmp_bst = bst_cmps - ac;

                    cpy = malloc(strlen(buf) + 1);
                    strcpy(cpy, buf);
                    strncpy(key, buf, 1);

                    ac = hc_cmps;
                    at = tick();
                    hc_add(hc, cpy, cpy);
                    ticks_hc = tick() - at;
                    cmp_hc = hc_cmps - ac;

                    if ((float)hc->cells / hc->n > 0.5)
                        hc_restruct(&hc);

                    ac = ho_cmps;
                    at = tick();
                    ho_add(ho, cpy, cpy);
                    ticks_ho = tick() - at;
                    cmp_ho = ho_cmps - ac;

                    if ((float)ho->els / ho->n > 0.3)
                        ho_restruct(&ho);
                }
                buf[0] = '\0';

                print_nodes_dot(favl, p);
                fclose(favl);
                system("dot -Tpng -o out/avl.png out/avl.dot");
                favl = fopen("./out/avl.dot", "w");
                print_bst_dot(fbst, bst);
                fclose(fbst);
                system("dot -Tpng -o out/bst.png out/bst.dot");
                fbst = fopen("./out/bst.dot", "w");
                break;

            case 2:
                printf("Key: ");
                scanf("%79s", buf);

                ac = hc_cmps;
                at = tick();
                hc_del(hc, buf);
                ticks_hc = tick() - at;
                cmp_hc = hc_cmps - ac;

                ac = ho_cmps;
                at = tick();
                ho_del(ho, buf);
                ticks_ho = tick() - at;
                cmp_ho = ho_cmps - ac;

                ac = bst_cmps;
                at = tick();
                bst = bst_del(bst, buf);
                ticks_bst = tick() - at;
                cmp_bst = bst_cmps - ac;

                ac = avl_cmps;
                at = tick();
                p = remove_key(p, buf);
                ticks_avl = tick() - at;
                cmp_avl = avl_cmps - ac;

                print_nodes_dot(favl, p);
                fclose(favl);
                system("dot -Tpng -o out/avl.png out/avl.dot");
                favl = fopen("./out/avl.dot", "w");
                print_bst_dot(fbst, bst);
                fclose(fbst);
                system("dot -Tpng -o out/bst.png out/bst.dot");
                fbst = fopen("./out/bst.dot", "w");
                break;

            case 3:
                printf("Key: ");
                scanf("%79s", buf);

                ac = hc_cmps;
                at = tick();
                printf("HC: %s\n", hc_get(hc, buf));
                ticks_hc = tick() - at;
                cmp_hc = hc_cmps - ac;

                ac = ho_cmps;
                at = tick();
                printf("HO: %s\n", ho_get(ho, buf));
                ticks_ho = tick() - at;
                cmp_ho = ho_cmps - ac;

                ac = bst_cmps;
                at = tick();
                printf("BST: %s\n", bst_get(bst, buf));
                ticks_bst = tick() - at;
                cmp_bst = bst_cmps - ac;

                ac = avl_cmps;
                at = tick();
                printf("AVL: %s\n", find_key(p, buf));
                ticks_avl = tick() - at;
                cmp_avl = avl_cmps - ac;
                break;

            case 4:
                printf("AVL: ");
                print_nodes_in(stdout, p);
                printf("\n\n");

                printf("BST: ");
                print_bst_in(stdout, bst);
                printf("\n");

                printf("\nClosed hash (efficiency: %.2f %d/%d):\n",
                        hc_efficiency(hc), hc->els, hc->cells);
                hc_print(hc);
                printf("\nOpened hash (efficiency: %.2f %d/%d):\n",
                        ho_efficiency(ho), ho->els, ho->cells);
                ho_print(ho);
                printf("\n");
                break;

            default:
                code = 0;
                break;
        }

        if (code && code != 4)
        {
            printf("Ticks opened hash: \033[1;32m%"PRIu64"\033[0m (%d)\n",
                   ticks_ho, cmp_ho);
            printf("Ticks closed hash: \033[1;32m%"PRIu64"\033[0m (%d)\n",
                   ticks_hc, cmp_hc);
            printf("Ticks AVL:         \033[1;32m%"PRIu64"\033[0m (%d)\n",
                   ticks_avl, cmp_avl);
            printf("Ticks BST:         \033[1;32m%"PRIu64"\033[0m (%d)\n",
                   ticks_bst, cmp_bst);
        }
    }
    while (code);

    fclose(favl);
    fclose(fbst);

    return 0;
}
