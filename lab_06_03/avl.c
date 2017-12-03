#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "avl.h"

node_t *node_init(char k)
{
    node_t *p = malloc(sizeof(node_t));
    if (!p)
        return NULL;

    p->key = k;
    p->h = 1;
    p->l = NULL;
    p->r = NULL;

    return p;
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

    if(bfactor(p)==2)
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

node_t *insert(node_t *p, char k)
{
    if(!p)
        return node_init(k);

    if(k<p->key)
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

node_t *remove_min(node_t *p)
{
    assert(p);

    if(p->l == 0)
        return p->r;

    p->l = remove_min(p->l);

    return balance(p);
}

node_t *remove_key(node_t *p, char k)
{
    if(!p)
        return 0;

    if(k < p->key)
        p->l = remove_key(p->l, k);
    else if(k > p->key)
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

void print_nodes(node_t *p)
{
    if (!p)
    {
        printf("\033[7;41;31m \033[0m");
        return;
    }

    if (!p->l && !p->r)
    {
        printf("%c", p->key);
        return;
    }

    printf("(");
    print_nodes(p->l);
    printf(" %c ", p->key);
    print_nodes(p->r);
    printf(")");
}
