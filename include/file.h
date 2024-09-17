#ifndef _file_h_
#define _file_h_

#include "../include/stack.h"

typedef struct noLine{
    unsigned char select;
    char *content;
    struct noLine *previous, *next;
}Lines;

typedef struct {
    unsigned line_marker;
    struct noLine *pointer_edit;
    char file_path[256];
    struct noLine *init;
    Deleted *del;
    Added *add;
}TextFile;

TextFile *create_file(const char *file_path);
void free_file(TextFile *file);
void print_file(TextFile *file);

#endif