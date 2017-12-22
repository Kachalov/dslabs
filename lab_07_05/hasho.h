#pragma once

#define HASHO_MAX 2.0

extern int ho_cmps;

typedef struct
{
    int deleted;
    char *k;
    char *v;
} hoe_t;
typedef hoe_t *hoe_pt;

typedef struct
{
    int n;
    int els;
    int cells;
    hoe_pt *data;
} ho_t;
typedef ho_t *ho_pt;


hoe_pt hoe_init(char *k , char *v);
ho_pt ho_init(int n);
uint64_t ho_hash(ho_pt h, char *k);
int ho_add(ho_pt h, char *k, char *v);
char *ho_get(ho_pt h, char *k);
void ho_del(ho_pt h, char *k);
void ho_print(ho_pt h);
void ho_restruct(ho_pt *h);
float ho_efficiency(ho_pt h);
