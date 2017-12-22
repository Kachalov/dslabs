#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "avl.h"

int avl_cmps = 0;

node_t *node_init(char *k)
{
    node_t *p = malloc(sizeof(node_t) + strlen(k) + 1);
    if (!p)
        return NULL;

    p->key = (void *)(p + 1);
    strcpy(p->key, k);
    p->h = 1;
    p->l = NULL;
    p->r = NULL;

    return p;
}

int node_key_cmp(char *pk, char *qk)
{
    assert(pk);
    assert(qk);

    avl_cmps++;
    return -strcmp(pk, qk);
}

uint8_t height(node_t *p)
{
    return p ? p->h : 0;
}

int bfactor(node_t *p)
{
    assert(p);

    return height(p->r) - height(p->l);
}

void fix_height(node_t *p)
{
    assert(p);

    uint8_t hl = height(p->l);
    uint8_t hr = height(p->r);

    p->h = (hl > hr ? hl : hr) + 1;
}

node_t *rotate_right(node_t *p)
{
    assert(p);

    node_t *q = p->l;

    p->l = q->r;
    q->r = p;

    fix_height(p);
    fix_height(q);

    return q;
}

node_t *rotate_left(node_t *q)
{
    assert(q);

    node_t *p = q->r;

    q->r = p->l;
    p->l = q;

    fix_height(q);
    fix_height(p);

    return p;
}

node_t *balance(node_t *p)
{
    assert(p);

    fix_height(p);

    if(bfactor(p) == 2)
    {
        if(bfactor(p->r) < 0)
            p->r = rotate_right(p->r);

        return rotate_left(p);
    }

    if(bfactor(p) == -2)
    {
        if(bfactor(p->l) > 0)
            p->l = rotate_left(p->l);

        return rotate_right(p);
    }

    return p;
}

node_t *insert(node_t *p, char *k)
{
    if(!p)
        return node_init(k);

    if(node_key_cmp(k, p->key) < 0)
        p->l = insert(p->l, k);
    else
        p->r = insert(p->r, k);

    return balance(p);
}

node_t *find_min(node_t *p)
{
    assert(p);

    return p->l ? find_min(p->l) : p;
}

char *find_key(node_t *p, char *k)
{
    if (!p)
        return NULL;

    if(node_key_cmp(k, p->key) < 0)
        return find_key(p->l, k);
    else if(node_key_cmp(k, p->key) > 0)
        return find_key(p->r, k);
    else
        return p->key;
}

node_t *remove_min(node_t *p)
{
    assert(p);

    if(p->l == 0)
        return p->r;

    p->l = remove_min(p->l);

    return balance(p);
}

node_t *remove_key(node_t *p, char *k)
{
    if(!p)
        return 0;

    if(node_key_cmp(k, p->key) < 0)
        p->l = remove_key(p->l, k);
    else if(node_key_cmp(k, p->key) > 0)
        p->r = remove_key(p->r, k);
    else
    {
        node_t *q = p->l;
        node_t *r = p->r;
        free(p);

        if(!r)
            return q;

        node_t *min = find_min(r);
        min->r = remove_min(r);
        min->l = q;

        return balance(min);
    }

    return balance(p);
}

node_t *remove_first_letter(node_t *p, char let)
{
    if (!p)
        return p;

    p->l = remove_first_letter(p->l, let);
    p->r = remove_first_letter(p->r, let);

    if (*p->key == let)
        p = remove_key(p, p->key);

    return p;
}

void print_nodes_pre(node_t *p)
{
    if (!p)
    {
        fprintf(stderr, "\033[7;41;31m \033[0m");
        return;
    }

    if (!p->l && !p->r)
    {
        fprintf(stderr, "%s", p->key);
        return;
    }

    fprintf(stderr, "(");
    fprintf(stderr, "%s ", p->key);
    print_nodes_pre(p->l);
    fprintf(stderr, " ");
    print_nodes_pre(p->r);
    fprintf(stderr, ")");
}

void print_nodes_in(node_t *p)
{
    if (!p)
    {
        fprintf(stderr, "\033[7;41;31m \033[0m");
        return;
    }

    if (!p->l && !p->r)
    {
        fprintf(stderr, "%s", p->key);
        return;
    }

    fprintf(stderr, "(");
    print_nodes_in(p->l);
    fprintf(stderr, " %s ", p->key);
    print_nodes_in(p->r);
    fprintf(stderr, ")");
}

void print_nodes_post(node_t *p)
{
    if (!p)
    {
        fprintf(stderr, "\033[7;41;31m \033[0m");
        return;
    }

    if (!p->l && !p->r)
    {
        fprintf(stderr, "%s", p->key);
        return;
    }

    fprintf(stderr, "(");
    print_nodes_post(p->l);
    fprintf(stderr, " ");
    print_nodes_post(p->r);
    fprintf(stderr, " %s", p->key);
    fprintf(stderr, ")");
}

void print_nodes_dot_rec(node_t *p)
{
    if (!p)
        return;

    if (!p->l && !p->r)
    {
        printf("    \"%s\";\n", p->key);
        return;
    }

    if (p->r)
    {
        printf("    \"%s\" -> \"%s\" [color=blue];\n", p->key, p->r->key);
        print_nodes_dot_rec(p->r);
    }

    if (p->l)
    {
        printf("    \"%s\" -> \"%s\" [color=red];\n", p->key, p->l->key);
        print_nodes_dot_rec(p->l);
    }
}

void print_nodes_dot(node_t *p)
{
    printf("digraph graphname {\n");
    print_nodes_dot_rec(p);
    printf("}\n");
}
