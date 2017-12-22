#pragma once

#define for_each(it, lst) for (list1_t *it = (lst); it; it = it->next)
#define list_print_int(lst) do {\
    for_each(it, lst){\
        printf("%d", *(int *)(it)->data);\
        if ((it)->next) printf(" ");\
    }\
    printf("\n");\
} while(0)

typedef struct node
{
    void *data;
    struct node *next;
} list1_t;

int list_init(list1_t **head);
void list_free(list1_t **head);
void delete_node_before(list1_t **head, list1_t *before);
void *pop_front(list1_t **head);
void list_insert(list1_t **head, list1_t *elem, list1_t *before);
int insert_data(list1_t **head, void *data);
list1_t *reverse(list1_t *head);
list1_t *sort(list1_t *head, int (*comparator)(const void *, const void *));
void sorted_insert(list1_t **head, list1_t *element,
                   int (*comparator)(const void *, const void *));
