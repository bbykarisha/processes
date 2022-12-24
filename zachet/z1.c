#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list_item {
    char *str;
    struct list_item *next, *prev;
};

int
duplicate(struct list_item *cur)
{
    if (cur == NULL) {
        return 1;
    }

    struct list_item *new_node = calloc(1, sizeof *new_node);
    if (new_node == NULL) {
        return 1;
    }

    new_node->str = cur->str;
    struct list_item *next = cur->next;
    cur->next = new_node;
    new_node->prev = cur;
    new_node->next = next;
    next->prev = new_node;

    return 0;
}

struct list_item *
move_to_end(struct list_item *list, struct list_item *moved)
{
    if ((list == NULL) || (moved == NULL)) {
        return NULL;
    }

    struct list_item *end = list->prev;
    struct list_item *next = moved->next;
    struct list_item *prev = moved->prev;

    moved->next->prev = prev;
    moved->prev->next = next;

    moved->next = list;
    moved->prev = end;

    list->prev->next = moved;
    list->prev = moved;

    return next;
}

void
process(struct list_item **list, const char *s)
{
    if ((list == NULL) || (s == NULL)) {
        return;
    }

    if (*list == NULL) {
        return;
    }

    struct list_item *end = (*list)->prev;
    struct list_item *cur = *list;

    int flag_start = 0;
    int flag_end = 0;

    if (cur->str == NULL) {
        return;
    }

    while (1) {
        if (cur == end) {
            flag_end = 1;
        }

        if (strstr(cur->str, s) != NULL) {
            for (int i = 0; i < strlen(cur->str); ++i) {
                duplicate(cur);
                cur = cur->next;
            }
            cur = cur->next;
            flag_start = 1;
        } else {
            if (flag_start == 0) {
                *list = cur->next;
                cur = cur->next;
            } else {
                cur = move_to_end(*list, cur);
            }
        }

        if (flag_end == 1) {
            break;
        }
    }

    return;
}
