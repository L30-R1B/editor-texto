#ifndef _file_h_
#define _file_h_

#include "../include/stack.h"

typedef struct noLine{
    char *content;
    struct noLine *previous, *next;
}Lines;

typedef struct {
    struct noLine *pointer_edit;
    char file_path[256];
    struct noLine *init;
    Deleted *del;
    Added *add;
}TextFile;

TextFile *create_file(const char *file_path);
void pointer_edit_move_up(TextFile *file);
void pointer_edit_move_down(TextFile *file);
void new_line(TextFile *file);
void add_line(TextFile *file, const char *content, int position);
void remove_line(TextFile *file, unsigned long index);
void free_file(TextFile *file);
void print_file(TextFile *file);

#endif