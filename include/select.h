#ifndef _select_h_
#define _select_h_

#include "../include/file.h"

void select_init(TextFile *f);
void select_reset(TextFile *f);

void select_current_line(TextFile *f);

void select_all(TextFile *f);
void select_interval(TextFile *f, unsigned start_line, unsigned final_line, unsigned start_interval, unsigned end_interval);
void select_print(TextFile *f);

#endif