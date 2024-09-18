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
#include "../include/select.h"

void select_init(TextFile *f){
    f->select_area->active = 1;
}

void select_reset(TextFile *f){
    f->select_area->active = 0;
}

void select_current_line(TextFile *f){
    if(f->pointer_line_edit == NULL)
        return;
    
    select_init(f);
}

void select_all(TextFile *f);

void select_interval(TextFile *f, unsigned start_line, unsigned final_line, unsigned start_interval, unsigned end_interval);

void select_print(TextFile *f);

