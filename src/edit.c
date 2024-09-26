#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

int pointer_line_edit_move_up(TextFile *file) {
    if (file->current_block->pointer_block != NULL) {
        unsigned len = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content);
        if(file->current_block->line_index > 0){
            file->current_block->line_index --;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1){
                    file->current_block->ch_index = len - 1;
                    return 1;
                }
        }else if(file->current_block->pointer_block->previous != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->previous;
            for(int i = SIZE_BLOCK - 1; i >= 0; i --){
                if(file->current_block->pointer_block->lines[i].content != NULL){
                    file->current_block->line_index = i;
                    break;
                }
            }
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1){
                    file->current_block->ch_index = len - 1;
                    return 1;
                }
        }
    } else {
        printf("Não é possível mover o ponteiro para cima. Linha já está no início ou lista está vazia.\n");
    }
    return 0;
}
int pointer_line_edit_move_down(TextFile *file) {
    if (file->current_block->pointer_block != NULL) {
        unsigned len = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content);
        if(file->current_block->line_index < SIZE_BLOCK - 1 && file->current_block->pointer_block->lines[file->current_block->line_index + 1].content != NULL){
            file->current_block->line_index ++;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1){
                    file->current_block->ch_index = len - 1;
                    return 1;
                }
        }else if(file->current_block->pointer_block->next != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->next;
            file->current_block->line_index = 0;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1){
                    file->current_block->ch_index = len - 1;
                    return 1;
                }
        }
    } else {
        printf("Não é possível mover o ponteiro para baixo. Linha já está no início ou lista está vazia.\n");
    }
    return 0;
}

void line_marker_edit_move_left(TextFile *file) {
    if (file->current_block->ch_index == 0) {
        if (pointer_line_edit_move_up(file)) {
            file->current_block->ch_index = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1;
        }
    } else {
        file->current_block->ch_index--;
    }
}

void line_marker_edit_move_right(TextFile *file) {
    if (file->current_block->ch_index >= strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1) {
        if (pointer_line_edit_move_down(file)) {
            file->current_block->ch_index = 0;
        }
    } else {
        file->current_block->ch_index++;
    }
}

void new_line(TextFile *file) {

    if (file->current_block->line_index > SIZE_BLOCK - 2) {
        Block *new_block = create_block();
        tie_blocks(file->current_block->pointer_block, new_block, file->current_block->pointer_block->next);

        file->current_block->pointer_block = new_block;
        file->current_block->line_index = 0;
    }else{
        file->current_block->line_index++;
        if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL){
            unsigned index = SIZE_BLOCK;
            unsigned char create = 0;
            for(unsigned i = SIZE_BLOCK - 1; i > file->current_block->line_index; i --){
                if(file->current_block->pointer_block->lines[i].content == NULL){
                    index = i;
                    break;
                }
            }
            create = index == SIZE_BLOCK ? 1 : 0;
            if(create){
                struct Block *new_block = create_block();
                tie_blocks(file->current_block->pointer_block, new_block, file->current_block->pointer_block->next);
                char *buffer = strdup(file->current_block->pointer_block->lines[SIZE_BLOCK - 1].content);

                for(unsigned i = SIZE_BLOCK - 1; i > file->current_block->line_index; i--){
                    free(file->current_block->pointer_block->lines[i].content);
                    file->current_block->pointer_block->lines[i].content = strdup(file->current_block->pointer_block->lines[i - 1].content);
                }
                new_block->lines[0].content = strdup(buffer);
                free(buffer);                
                new_block->number_lines = 1;
                concatenate_blocks(new_block, new_block->next);
            }else{
                for(unsigned i = index; i > file->current_block->line_index; i --){
                    file->current_block->pointer_block->lines[i].content = file->current_block->pointer_block->lines[i - 1].content;
                }
                file->current_block->pointer_block->lines[file->current_block->line_index].content = NULL;
            }
        }
    }

    if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
        free(file->current_block->pointer_block->lines[file->current_block->line_index].content);

    file->current_block->pointer_block->lines[file->current_block->line_index].content = strdup("");
    if (file->current_block->pointer_block->lines[file->current_block->line_index].content == NULL) {
        perror("Erro ao alocar memória para o conteúdo da nova linha");
        return;
    }

    file->current_block->ch_index = 0;
    if(file->current_block->pointer_block->number_lines != SIZE_BLOCK)
        file->current_block->pointer_block->number_lines ++;

}

void add_line(TextFile *file, const char *content, int position) {
    if (file == NULL || content == NULL || file->current_block == NULL) {
        perror("Arquivo, conteúdo ou bloco atual não inicializado");
        return;
    }

    Block *current_block = file->current_block->pointer_block;

    if (file->current_block->line_index >= SIZE_BLOCK) {
        perror("Índice da linha fora dos limites do bloco");
        return;
    }

    if (position == 0) {
        char *old_content = current_block->lines[file->current_block->line_index].content;
        
        if (old_content != NULL) {
            free(old_content);
        }else{
            current_block->number_lines ++;
        }
        current_block->lines[file->current_block->line_index].content = strdup(content);
        if (current_block->lines[file->current_block->line_index].content == NULL) {
            perror("Erro ao duplicar conteúdo");
            return;
        }
        return;
    } else {
        fprintf(stderr, "Inserção em posição diferente de 0 ainda não implementada.\n");
    }

    file->current_block->line_index ++;
}

void remove_line(TextFile *file, int position){
    if(file == NULL || file->current_block->pointer_block == NULL)
        return;
    
    if(!position){
        Block *block = file->current_block->pointer_block;
    
        if (block->lines[position].content != NULL) {
            free(block->lines[position].content);
            block->lines[position].content = NULL;
        }

        for (int index = position; index < block->number_lines - 1; index++) {
            block->lines[index].content = block->lines[index + 1].content;
            block->lines[index + 1].content = NULL;
        }
        block->number_lines--;
    }
}

void add_char(TextFile *file, const char ch, int mode){
    if(file == NULL)
        return;

    int index;
    int len = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content);

    if(mode == 1){
        if((int) file->current_block->ch_index + 1 > len - 1){
            if(pointer_line_edit_move_down(file)){
                len = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content);
                file->current_block->ch_index = 0;
            }
        }
        else{
            index = file->current_block->ch_index + 1;
        }
    }else if(mode == -1){
        if(((int) file->current_block->ch_index - 1) < 0){
            if(pointer_line_edit_move_up(file)){
                len = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content);
                file->current_block->ch_index = len - 1;
            }
        }
        else{
            index = file->current_block->ch_index - 1;
        }
    }else{
        index = file->current_block->ch_index;
    }

    char aux[len + 2];

    for(int i = 0, j = 0; i < len + 1; i ++){
        if(i == index){
            aux[i] = ch;
            continue;
        }
        
        aux[i] = file->current_block->pointer_block->lines[file->current_block->line_index].content[j];
        j ++;
    }
    
    aux[len + 1] = '\0';

    free(file->current_block->pointer_block->lines[file->current_block->line_index].content);
    file->current_block->pointer_block->lines[file->current_block->line_index].content = strdup(aux);
}

void remove_char(TextFile *file, int mode){
    if(file == NULL)
        return;
    
    if(!mode)
        return;
}