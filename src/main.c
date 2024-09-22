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
    for(int i = 0; i < 20; i ++)
        new_line(f);
    char *str[10] = {"aaa", "bbb", "ccc", "ddd", "eee", "fff"};
    int op;
    for(int i = 0; i < 10; i ++)
        pointer_line_edit_move_up(f);
    for(unsigned i = 0; i < 100; i ++){
        op = rand() % 2;
        if (op == 0) {
            for(int i = 0; i < rand() % 8 + 1; i ++)
                pointer_line_edit_move_up(f);
//            printf("\t( up ) ");
        } else{
            for(int i = 0; i < rand() % 8 + 1; i ++)
                pointer_line_edit_move_down(f);
  //          printf("\t(down) ");
        }
        op = rand() % 6;
    //    printf("insert %s (index B(%p) L(%u))\n", str[op], f->current_block->pointer_block, f->current_block->line_index);
        add_line(f, str[op], 0);
    }
}

int main(){
    srand(time(NULL));

    TextFile *f = create_file("teste.txt");

    test(f);

    print_file(f);
    new_line(f);
    new_line(f);
    new_line(f);
    print_file(f);


    free_file(f);

    return 0;
}