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
    for(int i = 0; i < 7; i ++)
        new_line(f);
    for(int i = 0; i < 4; i ++)
        pointer_line_edit_move_up(f);
}

int main(){
    unsigned seed = time(NULL);
    
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

    add_char(f, 'A', 1);
    print_file(f);

    free_file(f);

    printf("SEED (%u)\n", seed);
    return 0;
}