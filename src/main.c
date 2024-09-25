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
    for(int i = 0; i < 50; i ++)
        new_line(f);
    for(int i = 0; i < 25; i ++)
        pointer_line_edit_move_up(f);
}

int main(){


    unsigned seed = 1727275583;// time(NULL);
    srand(seed);

    TextFile *f = create_file("teste.txt");

    test(f);

    for(int i = 0; i < 50000; i ++){
        for(int j = 0; j < rand() % 25 + 1; j ++){
            pointer_line_edit_move_up(f);
        }
        for(int j = 0; j < rand() % 25 + 1; j ++){
            pointer_line_edit_move_down(f);
        }
        new_line(f);
    }

    print_file(f);

    free_file(f);

    printf("SEED (%u)\n", seed);
    return 0;
}