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

void test(TextFile *f){
    for(int i = 0; i < 24; i ++)
        new_line(f);
    for(int i = 0; i < 16; i ++)
        pointer_line_edit_move_up(f);
}

int main(){
    unsigned seed = 101;//time(NULL);
    
    srand(seed);

    TextFile *f = create_file("teste.txt");

    test(f);

    for(int i = 0; i < 500; i ++){
        for(int j = 0; j < rand() % 25 + 1; j ++){
            pointer_line_edit_move_up(f);
        }
        for(int j = 0; j < rand() % 25 + 1; j ++){
            pointer_line_edit_move_down(f);
        }
        add_line(f, "content", 0);
    }

    for(unsigned i = 0; i < 6; i ++)
        line_marker_edit_move_right(f);
    print_file(f);
    add_char(f, 'L', -1);
    add_char(f, 'E', -1);
    add_char(f, 'O', -1);
    add_char(f, 'N', -1);
    add_char(f, 'A', -1);
    add_char(f, 'R', -1);
    add_char(f, 'D', -1);
    add_char(f, 'O', -1);

    f->current_block->ch_index = 6;
    f->current_block->line_index = 7;


    print_file(f);
    printf("REMOVE\n");
    remove_char(f, -1);

    print_file(f);
    
    free_file(f);

    printf("SEED (%u)\n", seed);
    return 0;
}