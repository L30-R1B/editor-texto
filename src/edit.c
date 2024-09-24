#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

void pointer_line_edit_move_up(TextFile *file) {
    if (file->current_block->pointer_block != NULL) {
        if(file->current_block->line_index > 0){
            file->current_block->line_index --;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1)
                    file->current_block->ch_index = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1;
        }else if(file->current_block->pointer_block->previous != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->previous;
            for(int i = SIZE_BLOCK - 1; i >= 0; i --){
                if(file->current_block->pointer_block->lines[i].content != NULL){
                    file->current_block->line_index = i;
                    break;
                }
            }
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1)
                    file->current_block->ch_index = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1;
        }
    } else {
        printf("Não é possível mover o ponteiro para cima. Linha já está no início ou lista está vazia.\n");
    }
}
void pointer_line_edit_move_down(TextFile *file) {
    if (file->current_block->pointer_block != NULL) {
        if(file->current_block->line_index < SIZE_BLOCK - 1 && file->current_block->pointer_block->lines[file->current_block->line_index + 1].content != NULL){
            file->current_block->line_index ++;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1)
                    file->current_block->ch_index = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1;
        }else if(file->current_block->pointer_block->next != NULL){
            file->current_block->pointer_block = file->current_block->pointer_block->next;
            file->current_block->line_index = 0;
            if(file->current_block->pointer_block->lines[file->current_block->line_index].content != NULL)
                if(file->current_block->ch_index > strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1)
                    file->current_block->ch_index = strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1;
        }
    } else {
        printf("Não é possível mover o ponteiro para baixo. Linha já está no início ou lista está vazia.\n");
    }
}
void line_marker_edit_move_left(TextFile *file){
    if(!file->current_block->ch_index){
        pointer_line_edit_move_up(file);
        return;
    }
    file->current_block->ch_index --;
}
void line_marker_edit_move_right(TextFile *file){
    if(file->current_block->ch_index >= (strlen(file->current_block->pointer_block->lines[file->current_block->line_index].content) - 1)){
        pointer_line_edit_move_down(file);
        return;
    }
    file->current_block->ch_index ++;
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
            for(unsigned i = SIZE_BLOCK - 1; i >= file->current_block->line_index; i --){
                if(file->current_block->pointer_block->lines[i].content == NULL){
                    index = i;
                    break;
                }
            }
            create = index == SIZE_BLOCK ? 1 : 0;
            if(create){
                struct Block *new_block = create_block();
                tie_blocks(file->current_block->pointer_block, new_block, file->current_block->pointer_block->next);
                char *buffer = file->current_block->pointer_block->lines[SIZE_BLOCK - 1].content;

                for(unsigned i = SIZE_BLOCK - 1; i > file->current_block->line_index; i--){
                    file->current_block->pointer_block->lines[i].content = strdup(file->current_block->pointer_block->lines[i - 1].content);
                }                
                new_block->lines[0].content = strdup(buffer);
                new_block->number_lines = 1;
            }else{
                for(unsigned i = index; i > file->current_block->line_index; i --){
                    file->current_block->pointer_block->lines[i].content = file->current_block->pointer_block->lines[i - 1].content;
                }
            }
        }
    }

    Line *new_line = &file->current_block->pointer_block->lines[file->current_block->line_index];

    printf("insert block(%p);  line (%u); content (%s)\n", (void*) file->current_block->pointer_block, file->current_block->line_index, "");
    new_line->content = strdup("");
    if (new_line->content == NULL) {
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
void remove_line(TextFile *file, unsigned long index) {
    if (file == NULL || file->init_block == NULL || file->current_block == NULL) {
        printf("O arquivo está vazio ou não inicializado.\n");
        return;
    }

    Block *current_block = file->init_block;
    unsigned long line_count = 0;
    unsigned long block_index = 0;

    // Navegar até o bloco e a linha correspondente ao índice fornecido
    while (current_block != NULL) {
        for (unsigned i = 0; i < SIZE_BLOCK; i++) {
            if (line_count == index) {
                Line *current_line = &current_block->lines[i];
                
                if (current_line->content == NULL) {
                    printf("Linha vazia, não encontrada no índice %lu.\n", index);
                    return;
                }

                // Liberar o conteúdo da linha
                free(current_line->content);
                current_line->content = NULL;

                // Ajustar as linhas subsequentes no bloco para preencher o espaço vazio
                for (unsigned j = i; j < SIZE_BLOCK - 1; j++) {
                    current_block->lines[j].content = current_block->lines[j + 1].content;
                }

                // Se for o último bloco e última linha, remover o bloco, se estiver vazio
                if (i == SIZE_BLOCK - 1 && current_block->next == NULL) {
                    current_block->lines[SIZE_BLOCK - 1].content = NULL; // Apaga o último conteúdo

                    // Verificar se o bloco está vazio
                    int is_empty = 1;
                    for (unsigned k = 0; k < SIZE_BLOCK; k++) {
                        if (current_block->lines[k].content != NULL) {
                            is_empty = 0;
                            break;
                        }
                    }

                    if (is_empty) {
                        if (current_block->previous != NULL) {
                            current_block->previous->next = NULL;
                        } else {
                            file->init_block = NULL; // Se era o único bloco
                        }

                        free(current_block);
                    }
                }

                printf("Linha no índice %lu removida.\n", index);
                return;
            }
            line_count++;
        }
        current_block = current_block->next;
        block_index++;
    }

    printf("Linha não encontrada no índice %lu.\n", index);
}
