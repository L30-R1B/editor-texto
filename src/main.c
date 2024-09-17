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

int main(){
    TextFile *f = create_file("teste.txt");

    add_line(f, "AAA", 1);
    add_line(f, "BBB", 1);
    add_line(f, "CCC", 1);
    add_line(f, "DDD", 1);
    add_line(f, "EEE", 1);
    add_line(f, "FFF", 1);
    add_line(f, "GGG", 1);
    add_line(f, "HHH", 1);

    pointer_edit_move_up(f);
    pointer_edit_move_up(f);
    pointer_edit_move_up(f);
    pointer_edit_move_up(f);
    pointer_edit_move_up(f);
    pointer_edit_move_up(f);

    add_line(f, "ZZZ", 0);

    new_line(f);

    print_file(f);

    free_file(f);

    return 0;
}