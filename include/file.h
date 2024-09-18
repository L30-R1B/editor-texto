#ifndef _file_h_
#define _file_h_

#include "../include/stack.h"

typedef struct noLine{
    char *content;
    struct noLine *previous, *next;
}Lines;

typedef struct {
    unsigned char active;
    unsigned line_start;
    unsigned line_start_marker;
    unsigned line_end;
    unsigned line_end_marker;
}SelectArea;

typedef struct {
    unsigned line_marker_edit;
    struct noLine *pointer_line_edit;
    unsigned index_pointer_line;
    char file_path[256];
    struct noLine *init;
    Deleted *del;
    Added *add;
    SelectArea *select_area;
}TextFile;

TextFile *create_file(const char *file_path);
void free_file(TextFile *file);
void print_file(TextFile *file);

#endif