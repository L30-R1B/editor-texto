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
                if(file->current_block->ch_index > len - 1)
                    file->current_block->ch_index = len - 1;
            return 1;
        }else if(file->current_block->pointer_block->previous != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->previous;
            for(int i = SIZE_BLOCK - 1; i >= 0; i --){
                if(file->current_block->pointer_block->lines[i].content != NULL){
                    file->current_block->line_index = i;
                    break;
                }
            }
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1)
                    file->current_block->ch_index = len - 1;
            return 1;
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
                if(file->current_block->ch_index > len - 1)
                    file->current_block->ch_index = len - 1;
            return 1;
        }else if(file->current_block->pointer_block->next != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->next;
            file->current_block->line_index = 0;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > len - 1)
                    file->current_block->ch_index = len - 1;
            return 1;
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

void add_char(TextFile *file, const char ch, int mode) {
    if (!file || !file->current_block || !file->current_block->pointer_block)
        return;

    char *content = file->current_block->pointer_block->lines[file->current_block->line_index].content;
    int len = strlen(content);
    if (!len) return;

    int index = file->current_block->ch_index;
    if (mode == 1) {
        index = index + 1 < len ? index + 1 : len;
    }

    content = (char*) realloc(content, len + 2);

    memmove(content + index + 1, content + index, len - index + 1);

    content[index] = ch;

    file->current_block->pointer_block->lines[file->current_block->line_index].content = content;
    file->current_block->ch_index = index;
}

void remove_char(TextFile *file, int mode) {
    if (!file || !file->current_block || !file->current_block->pointer_block) {
        return;
    }

    int index = file->current_block->ch_index;
    char *current_content = file->current_block->pointer_block->lines[file->current_block->line_index].content;
    int current_content_len = strlen(current_content);

    if (mode == 1) {
        index++;
    }

    if (index == 0) {
        if (!file->current_block->line_index) {
            Block *previous_block = file->current_block->pointer_block->previous;
            if (previous_block != NULL) {
                Line *previous_line = &previous_block->lines[previous_block->number_lines - 1];
                char *previous_content = previous_line->content;
                int previous_content_len = strlen(previous_content);
                char *new_concat_content = realloc(previous_content, (previous_content_len + current_content_len + 1) * sizeof(char));

                if (new_concat_content == NULL) {
                    return;
                }

                previous_line->content = new_concat_content;
                strcat(previous_line->content, current_content);
                free(current_content);

                file->current_block->pointer_block->lines[file->current_block->line_index].content = previous_line->content;
                previous_line->content = NULL;
                previous_block->number_lines --;
                file->current_block->ch_index = previous_content_len;
            }else{
                return;
            }
        }else {
            Line *previous_line = &file->current_block->pointer_block->lines[file->current_block->line_index - 1];
            char *previous_content = previous_line->content;
            int previous_content_len = strlen(previous_content);
            char *new_concat_content = realloc(previous_content, (previous_content_len + current_content_len + 1) * sizeof(char));
            if (new_concat_content == NULL) {
                return;
            }
            previous_line->content = new_concat_content;

            strcat(previous_line->content, current_content);

            free(current_content);

            for (int i = file->current_block->line_index; i < file->current_block->pointer_block->number_lines - 1; i++) {
                file->current_block->pointer_block->lines[i].content = file->current_block->pointer_block->lines[i + 1].content;
            }

            file->current_block->pointer_block->number_lines--;
            file->current_block->pointer_block->lines[file->current_block->pointer_block->number_lines].content = NULL;

            file->current_block->line_index --;
            file->current_block->ch_index = previous_content_len;
        }
    }else if(index == current_content_len){
        
    }
}

