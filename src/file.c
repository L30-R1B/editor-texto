#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

TextFile *create_file(const char *file_path) {
    TextFile *file = (TextFile *) malloc(sizeof(TextFile));
    if (file == NULL) {
        perror("Erro ao alocar memória para File");
        return NULL;
    }

    file->add = (Trash*) malloc(sizeof(Trash));
    init_stack(file->add);

    file->del = (Trash*) malloc(sizeof(Trash));
    init_stack(file->del);

    file->select_area = (SelectArea*) malloc(sizeof(SelectArea));
    file->select_area->active = 0;
    file->select_area->line_end = 0;
    file->select_area->line_end_marker = 0;
    file->select_area->line_start = 0;
    file->select_area->line_start_marker = 0;

    file->line_marker_edit = 0;
    
    strncpy(file->file_path, file_path, sizeof(file->file_path) - 1);
    file->file_path[sizeof(file->file_path) - 1] = '\0';
    file->pointer_line_edit = NULL;
    file->index_pointer_line = 0;
    file->init = NULL;

    return file;
}

void free_file(TextFile *file) {
    Lines *current = file->init;
    while (current != NULL) {
        Lines *next = current->next;
        free(current->content);
        free(current);
        current = next;
    }

    if(file->add != NULL){
        free_stack(file->add);
        free(file->add);
    }
    if(file->del != NULL){
        free_stack(file->del);
        free(file->del);
    }

    if(file->select_area != NULL)
        free(file->select_area);

    free(file);
}

void print_file(TextFile *file) {
    Lines *current = file->init;
    printf("__________________________________________________________\n");
    printf("\tADD\n");
    print_stack(file->add);
    printf("---------------------\n");    
    printf("\tDEL\n");
    print_stack(file->del);
    printf("---------------------\n");
    printf("ARQ CONTEUDO :\n");
    while (current != NULL) {
        printf("\t %s", current->content);
        current = current->next;
    }
    printf("---------------------\n");
    printf("__________________________________________________________\n");
}
