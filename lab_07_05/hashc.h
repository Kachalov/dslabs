#pragma once

#include "lib/list.h"

typedef struct
{
    char *k;
    char *v;
} hce_t;
typedef hce_t *hce_pt;

typedef struct
{
    int n;
    int els;
    int cells;
    list1_t **data;
} hc_t;
typedef hc_t *hc_pt;


hce_pt hce_init(char *k , char *v);
hc_pt hc_init(int n);
int hc_hash(hc_pt h, char *k);
int hc_add(hc_pt h, char *k, char *v);
char *hc_get(hc_pt h, char *k);
void hc_del(hc_pt h, char *k);
void hc_print(hc_pt h);
