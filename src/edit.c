#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

void pointer_edit_move_up(TextFile *file) {
    if (file->pointer_edit != NULL && file->pointer_edit->previous != NULL) {
        file->pointer_edit = file->pointer_edit->previous;
        if(file->line_marker >= (strlen(file->pointer_edit->content) - 1))
            file->line_marker = (strlen(file->pointer_edit->content) - 1);
    } else {
        printf("Não é possível mover o ponteiro para cima. Linha já está no início ou lista está vazia.\n");
    }
}

void pointer_edit_move_down(TextFile *file) {
    if (file->pointer_edit != NULL && file->pointer_edit->next != NULL) {
        file->pointer_edit = file->pointer_edit->next;
        if(file->line_marker >= (strlen(file->pointer_edit->content) - 1))
            file->line_marker = (strlen(file->pointer_edit->content) - 1);
    } else {
        printf("Não é possível mover o ponteiro para baixo. Linha já está no fim ou lista está vazia.\n");
    }
}

void line_marker_move_left(TextFile *file){
    if(!file->line_marker)
        return;
    file->line_marker --;
}
void line_marker_move_right(TextFile *file){
    if(file->line_marker >= (strlen(file->pointer_edit->content) - 1))
        return;
    file->line_marker ++;
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
    new_line->select = 0;
    
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
    new_line->select = 0;
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
                    file->pointer_edit->select = 0;
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