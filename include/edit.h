#ifndef _edit_h_
#define _edit_h_

#include "../include/file.h"

void pointer_line_edit_move_up(TextFile *file);
void pointer_line_edit_move_down(TextFile *file);
void line_marker_edit_move_left(TextFile *file);
void line_marker_edit_move_right(TextFile *file);

void new_line(TextFile *file);
void add_line(TextFile *file, const char *content, int position);
void remove_line(TextFile *file, unsigned long index);

#endif