#include <inttypes.h>

typedef struct _node_t
{
    char *key;
    uint8_t h;
    struct _node_t *l;
    struct _node_t *r;
} node_t;

node_t *node_init(char *k);
uint8_t height(node_t *p);
int bfactor(node_t *p);
void fix_height(node_t *p);
node_t *rotate_right(node_t *p);
node_t *rotate_left(node_t *q);
node_t *balance(node_t *p);
node_t *insert(node_t *p, char *k);
node_t *find_min(node_t *p);
node_t *remove_min(node_t *p);
node_t *remove_key(node_t *p, char *k);
node_t *remove_first_letter(node_t *p, char let);
void print_nodes_pre(node_t *p);
void print_nodes_in(node_t *p);
void print_nodes_post(node_t *p);
void print_nodes_dot_rec(node_t *p);
void print_nodes_dot(node_t *p);
