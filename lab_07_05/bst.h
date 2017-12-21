#pragma once

#include "lib/list.h"

typedef struct _bst_t
{
    char *k;
    char *v;
    struct _bst_t *l;
    struct _bst_t *r;
} bst_t;

typedef bst_t *bst_pt;

bst_pt bst_init(char *k, char *v);
bst_pt bst_add(bst_pt bst, char *k, char *v);
bst_pt bst_del(bst_pt bst, char *k);
char *bst_get(bst_pt bst, char *k);
bst_pt bst_remove_first_letter(bst_pt bst, char let);
void bst_find_first_letter(bst_pt bst, char let, list1_t **ret);
void print_bst_in(bst_pt bst);

