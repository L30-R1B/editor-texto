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

    add_line(f, "AAA\n", 1);
    add_line(f, "BBB\n", -1);
    add_line(f, "CCC\n", 0);
    add_line(f, "DDD\n", 1);
    add_line(f, "EEE\n", -1);
    add_line(f, "FFF\n", -1);
    add_line(f, "GGG\n", 0);
    add_line(f, "HHH\n", 1);

    printf("(%u) %s", f->index_pointer_line, f->pointer_line_edit->content);
    add_line(f, "Teste\n", -1);
    printf("(%u) %s", f->index_pointer_line, f->pointer_line_edit->content);

    print_file(f);

    free_file(f);

    return 0;
}