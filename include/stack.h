#ifndef _stack_h_
#define _stack_h_

#define CAPACITY_RETURNS 256

typedef struct {
    long unsigned index;
    char *content;
}LinesChanged;

typedef struct {
    LinesChanged lc[CAPACITY_RETURNS];
    unsigned size;
    unsigned start;
}Trash, Deleted, Added;

void init_stack(Trash *t);
int is_full(const Trash *t);
int is_empty(const Trash *t);
void push(Trash *t, long unsigned index, const char *content);
LinesChanged pop(Trash *t);
void free_stack(Trash *t);
void print_stack(const Trash *t);

#endif