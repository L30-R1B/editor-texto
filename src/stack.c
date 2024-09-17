#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"
#include "../include/stack.h"

void init_stack(Trash *t) {
    t->size = 0;
    t->start = 0;
    memset(t->lc, 0, sizeof(t->lc));
}

int is_full(const Trash *t) {
    return t->size == CAPACITY_RETURNS;
}

int is_empty(const Trash *t) {
    return t->size == 0;
}

void push(Trash *t, long unsigned index, const char *content) {
    if (is_full(t)) {
        t->lc[t->start].index = index;
        free(t->lc[t->start].content);
        t->lc[t->start].content = strdup(content);
        t->start = (t->start + 1) % CAPACITY_RETURNS;
    } else {
        unsigned pos = (t->start + t->size) % CAPACITY_RETURNS;
        t->lc[pos].index = index;
        t->lc[pos].content = strdup(content);
        t->size++;
    }
}

LinesChanged pop(Trash *t) {
    LinesChanged removed = {0, NULL};
    
    if (is_empty(t)) {
        printf("Pilha vazia. Não há elementos para remover.\n");
        return removed;
    }

    unsigned pos = t->size - 1;
    removed = t->lc[pos];
    t->size--;

    if (removed.content != NULL) {
        free(removed.content);
    }

    return removed;
}

void free_stack(Trash *t) {
    for (unsigned i = 0; i < CAPACITY_RETURNS; i++) {
        if (t->lc[i].content != NULL) {
            free(t->lc[i].content);
        }
    }
}

void print_stack(const Trash *t) {
    printf("---------------------------------\n");
    if (is_empty(t)) {
        printf("\tPilha vazia.\n");
        return;
    }

    printf("\tConteúdo da pilha:\n");
    for (unsigned i = 0; i < t->size; i++) {
        unsigned pos = (t->start + i) % CAPACITY_RETURNS;
        printf("\t\tIndex: %lu, Content: %s\n", t->lc[pos].index, t->lc[pos].content);
    }
    printf("---------------------------------\n");
}