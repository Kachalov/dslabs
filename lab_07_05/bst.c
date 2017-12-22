#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bst.h"

int bst_cmps = 0;

bst_pt bst_init(char *k, char *v)
{
    bst_pt p = malloc(sizeof(bst_t) + strlen(k) + strlen(v) + 2);
    if (!p)
        return NULL;

    p->k = (void *)(p + 1);
    p->v = p->k + strlen(k) + 1;
    strcpy(p->k, k);
    strcpy(p->v, v);
    p->l = NULL;
    p->r = NULL;

    return p;
}

bst_pt bst_add(bst_pt bst, char *k, char *v)
{
    if(!bst)
        bst = bst_init(k, v);
    else
    {
        bst_cmps++;
        if(strcmp(k, bst->k) < 0)
            bst->l = bst_add(bst->l, k, v);
        else
            bst->r = bst_add(bst->r, k, v);
    }

    return bst;
}

bst_pt bst_del(bst_pt bst, char *k)
{
    bst_pt origin = bst;

    if(!bst)
        return bst;

    bst_cmps++;
    if(strcmp(k, bst->k) < 0)
        bst->l = bst_del(bst->l, k);
    else if (strcmp(k, bst->k) > 0)
        bst->r = bst_del(bst->r, k);
    else
    {
        if (!bst->l && !bst->r)
            bst = NULL;
        else if (!bst->l)
            bst = bst->r;
        else if (!bst->r)
            bst = bst->l;
        else
        {
            if (!bst->r->l)
                bst = bst->r->r;
            else
            {
                bst_pt cursor = bst->r;
                bst_pt parent = NULL;

                while (cursor->l)
                {
                    parent = cursor;
                    cursor = cursor->l;
                }

                strcpy(bst->k, cursor->k);
                strcpy(bst->v, cursor->v);

                if (parent)
                    parent->l = bst_del(parent->l, k);
                else
                    bst->r = bst_del(bst->r, k);
            }
        }
        free(origin);
    }

    return bst;
}

char *bst_get(bst_pt bst, char *k)
{
    char *ret = NULL;

    if(!bst)
        ret = NULL;
    else
    {
        bst_cmps++;
        if(strcmp(k, bst->k) < 0)
            ret = bst_get(bst->l, k);
        else if(strcmp(k, bst->k) > 0)
            ret = bst_get(bst->r, k);
        else
            return bst->k;
    }

    return ret;
}

bst_pt bst_remove_first_letter(bst_pt bst, char let)
{
    if (!bst)
        return bst;

    bst->l = bst_remove_first_letter(bst->l, let);
    bst->r = bst_remove_first_letter(bst->r, let);

    bst_cmps++;
    if (*bst->k == let)
        bst = bst_del(bst, bst->k);

    return bst;
}

void bst_find_first_letter(bst_pt bst, char let, list1_t **ret)
{
    if (!bst)
        return;

    bst_find_first_letter(bst->l, let, ret);
    bst_find_first_letter(bst->r, let, ret);

    bst_cmps++;
    if (*bst->k == let)
        insert_data(ret, bst->k);
}

void print_bst_in(bst_pt bst)
{
    if (!bst)
    {
        fprintf(stderr, "\033[7;41;31m \033[0m");
        return;
    }

    if (!bst->l && !bst->r)
    {
        fprintf(stderr, "%s", bst->k);
        return;
    }

    fprintf(stderr, "(");
    print_bst_in(bst->l);
    fprintf(stderr, " %s ", bst->k);
    print_bst_in(bst->r);
    fprintf(stderr, ")");
}
