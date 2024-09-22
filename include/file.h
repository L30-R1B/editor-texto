#ifndef _file_h_
#define _file_h_

#include "../include/stack.h"

#define SIZE_BLOCK 8

typedef struct {
    char *content;
}Line;

typedef struct Block{
    unsigned char number_lines;
    Line lines[SIZE_BLOCK];
    struct Block *previous, *next;
}Block;

typedef struct{
    struct Block *pointer_block;
    unsigned line_index;
    unsigned ch_index;
}BlockMarker;

typedef struct {
    unsigned char active;
    BlockMarker line_start;
    BlockMarker line_end;
}SelectArea;

typedef struct {
    char file_path[256];
    struct Block *init_block;
    BlockMarker *current_block;
    SelectArea *selection_area;
    Deleted *del;
    Added *add;
}TextFile;

struct Block *create_block();
TextFile *create_file(const char *file_path);
void tie_blocks(Block *previous, Block *current, Block *next);
void free_file(TextFile *file);
void print_file(TextFile *file);

#endif