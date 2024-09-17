#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

void free_line(Lines *line) {
    if (line != NULL) {
        free(line->content);
        free(line);
    }
}

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

    strncpy(file->file_path, file_path, sizeof(file->file_path) - 1);
    file->file_path[sizeof(file->file_path) - 1] = '\0';
    file->pointer_edit = NULL;
    file->init = NULL;

    return file;
}

void pointer_edit_move_up(TextFile *file) {
    if (file->pointer_edit != NULL && file->pointer_edit->previous != NULL) {
        file->pointer_edit = file->pointer_edit->previous;
    } else {
        printf("Não é possível mover o ponteiro para cima. Linha já está no início ou lista está vazia.\n");
    }
}

void pointer_edit_move_down(TextFile *file) {
    if (file->pointer_edit != NULL && file->pointer_edit->next != NULL) {
        file->pointer_edit = file->pointer_edit->next;
    } else {
        printf("Não é possível mover o ponteiro para baixo. Linha já está no fim ou lista está vazia.\n");
    }
}

void new_line(TextFile *file) {
    Lines *new_line = (Lines *) malloc(sizeof(Lines));
    if (new_line == NULL) {
        perror("Erro ao alocar memória para nova linha");
        return;
    }
    
    new_line->content = strdup("\n");
    if (new_line->content == NULL) {
        perror("Erro ao alocar memória para o conteúdo da nova linha");
        free(new_line);
        return;
    }
    
    new_line->previous = NULL;
    new_line->next = NULL;

    if (file->pointer_edit != NULL) {
        new_line->next = file->pointer_edit->next;
        new_line->previous = file->pointer_edit;
        
        if (file->pointer_edit->next != NULL) {
            file->pointer_edit->next->previous = new_line;
        }
        
        file->pointer_edit->next = new_line;
    } else {
        new_line->next = file->init;
        if (file->init != NULL) {
            file->init->previous = new_line;
        }
        file->init = new_line;
    }
    
    file->pointer_edit = new_line;
}

void add_line(TextFile *file, const char *content, int position) {
    Lines *new_line = (Lines *) malloc(sizeof(Lines));
    if (new_line == NULL) {
        perror("Erro ao alocar memória para Lines");
        return;
    }
    
    new_line->content = strdup(content);
    new_line->previous = NULL;
    new_line->next = NULL;

    if (file->init == NULL) {
        file->init = new_line;
        file->pointer_edit = new_line;
    } else {
        if (file->pointer_edit != NULL) {
            if (position == -1) {
                new_line->next = file->pointer_edit;
                new_line->previous = file->pointer_edit->previous;

                if (file->pointer_edit->previous != NULL) {
                    file->pointer_edit->previous->next = new_line;
                } else {
                    file->init = new_line;
                }
                file->pointer_edit->previous = new_line;
            } else if (position == 0) {
                if (file->pointer_edit != NULL) {
                    free(file->pointer_edit->content);
                    file->pointer_edit->content = strdup(content);
                    free(new_line->content);
                    free(new_line);
                    return;
                } else {
                    new_line->next = file->init;
                    if (file->init != NULL) {
                        file->init->previous = new_line;
                    }
                    file->init = new_line;
                }
            } else if (position == 1) {
                new_line->next = file->pointer_edit->next;
                new_line->previous = file->pointer_edit;

                if (file->pointer_edit->next != NULL) {
                    file->pointer_edit->next->previous = new_line;
                }

                file->pointer_edit->next = new_line;
            }
        } else {
            new_line->next = file->init;
            if (file->init != NULL) {
                file->init->previous = new_line;
            }
            file->init = new_line;
        }

        if (position != 0) {
            file->pointer_edit = new_line;
        }
    }
}


void remove_line(TextFile *file, unsigned long index) {
    if (file->init == NULL) {
        printf("O arquivo está vazio.\n");
        return;
    }

    Lines *current = file->init;
    unsigned long count = 0;

    while (current != NULL && count < index) {
        current = current->next;
        count++;
    }

    if (current == NULL) {
        printf("Linha não encontrada.\n");
        return;
    }
    
    if (current->previous != NULL) {
        current->previous->next = current->next;
    } else {
        file->init = current->next;
    }

    if (current->next != NULL) {
        current->next->previous = current->previous;
    }

    file->pointer_edit = current->previous;

    free(current->content);
    free(current);
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
        printf("\t%s", current->content);
        current = current->next;
    }
    printf("---------------------\n");
    printf("__________________________________________________________\n");
}
