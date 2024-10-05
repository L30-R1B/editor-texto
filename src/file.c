#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/open.h"
#include "../include/save.h"
#include "../include/edit.h"
#include "../include/file.h"

char *buffer;

TextFile *create_file(const char *file_path) {
    TextFile *file = (TextFile *) malloc(sizeof(TextFile));
    if (file == NULL) {
        perror("Erro ao alocar memória para TextFile");
        return NULL;
    }

    file->current_block = (BlockMarker*) malloc(sizeof(BlockMarker));
    if (file->current_block == NULL) {
        perror("Erro ao alocar memória para BlockMarker");
        free(file);
        return NULL;
    }
    file->current_block->ch_index = 0;
    file->current_block->line_index = 0;
    file->current_block->pointer_block = NULL;

    file->add = (Trash*) malloc(sizeof(Trash));
    if (file->add == NULL) {
        perror("Erro ao alocar memória para Trash (adicionadas)");
        free(file->current_block);
        free(file);
        return NULL;
    }
    init_stack(file->add);

    file->del = (Trash*) malloc(sizeof(Trash));
    if (file->del == NULL) {
        perror("Erro ao alocar memória para Trash (deletadas)");
        free(file->add);
        free(file->current_block);
        free(file);
        return NULL;
    }
    init_stack(file->del);

    file->selection_area = (SelectArea*) malloc(sizeof(SelectArea));
    if (file->selection_area == NULL) {
        perror("Erro ao alocar memória para SelectArea");
        free(file->add);
        free(file->del);
        free(file->current_block);
        free(file);
        return NULL;
    }
    file->selection_area->active = 0; 

    strncpy(file->file_path, file_path, sizeof(file->file_path) - 1);
    file->file_path[sizeof(file->file_path) - 1] = '\0';

    file->init_block = (Block*) malloc(sizeof(Block));
    if (file->init_block == NULL) {
        perror("Erro ao alocar memória para o bloco inicial");
        free(file->selection_area);
        free(file->add);
        free(file->del);
        free(file->current_block);
        free(file);
        return NULL;
    }
    for (unsigned i = 1; i < SIZE_BLOCK; i++) {
        file->init_block->lines[i].content = NULL;
    }

    Line *new_line = &file->init_block->lines[file->current_block->line_index];
    new_line->content = strdup("");
    file->init_block->number_lines = 1;

    file->init_block->next = NULL;
    file->init_block->previous = NULL;
    file->current_block->pointer_block = file->init_block;

    return file;
}

struct Block *create_block(){
    struct Block *new = (struct Block *) malloc(sizeof(struct Block));
    new->next = NULL;
    new->previous = NULL;
    new->number_lines = 0;
    for(unsigned i = 0; i < SIZE_BLOCK; i ++)
        new->lines[i].content = NULL;

    return new;
}

void free_block(Block *block) {
    if (block != NULL) {
        for (unsigned i = 0; i < SIZE_BLOCK; i++) {
            if (block->lines[i].content != NULL) {
                free(block->lines[i].content);
            }
        }
        free(block);
    }
}

void tie_blocks(Block *previous, Block *current, Block *next) {
    if (previous != NULL) {
        previous->next = current;
    }

    if (current != NULL) {
        current->previous = previous;
        current->next = next;
    }

    if (next != NULL) {
        next->previous = current;
    }
}

void concatenate_blocks(Block *first_block, Block *second_block){
    if(first_block == NULL || second_block == NULL)
        return;
    
    if(first_block->number_lines == SIZE_BLOCK || second_block->number_lines == SIZE_BLOCK)
        return;

    int index;
    for(index = 0; index < second_block->number_lines; index ++){
        if(first_block->number_lines == SIZE_BLOCK)
            break;

        first_block->lines[first_block->number_lines].content = strdup(second_block->lines[index].content);
        first_block->number_lines ++;

        if(index < second_block->number_lines - 1){
            free(second_block->lines[index].content);
            second_block->lines[index].content = strdup(second_block->lines[index + 1].content);
        }
    }
    
    second_block->number_lines -= index;

    if(!second_block->number_lines){
        Block *aux_block = second_block->next;
        free_block(second_block);
        tie_blocks(first_block->previous, first_block, aux_block);
    }

}

void free_file(TextFile *file) {
    if (file == NULL) {
        return;
    }

    Block *current = file->init_block;
    while (current != NULL) {
        //printf("Liberando bloco %p\n", (void*) current);
        Block *next = current->next;
        free_block(current);
        current = next;
    }
    if (file->add != NULL) {
        free_stack(file->add);
    }
    if (file->del != NULL) {
        free_stack(file->del);
    }

    if (file->selection_area != NULL) {
        free(file->selection_area);
    }
    
    if(file->current_block != NULL)
        free(file->current_block);

    free(file);
}

void print_file(TextFile *file) {
    if (file == NULL) {
        printf("Arquivo vazio ou não inicializado.\n");
        return;
    }

    Block *current = file->init_block;

    printf("__________________________________________________________\n");
    
    /*printf("\tADD\n");
    if (file->add != NULL) {
        print_stack(file->add);
    } else {
        printf("\tNenhuma operação de adição.\n");
    }
    
    printf("---------------------\n");
    
    printf("\tDEL\n");
    if (file->del != NULL) {
        print_stack(file->del);
    } else {
        printf("\tNenhuma operação de exclusão.\n");
    }
    
    printf("---------------------\n");
    */
    printf("ARQ CONTEUDO :\n");
    unsigned cont = 0;
    
        printf("\tChar index : %u\n", file->current_block->ch_index);
    while (current != NULL) {
        printf("\tBloco %u; Qtd linhas %u; %s : \n", cont, current->number_lines, (current == file->current_block->pointer_block ? "(B atual)" : ""));
        for (unsigned current_line = 0; current_line < SIZE_BLOCK; current_line++) {
            if (current->lines[current_line].content != NULL) {
                printf("\t\t(%u)(%p, %p) %s\t", current_line, (void*) current, (void*) current->lines[current_line].content, ((current == file->current_block->pointer_block && current_line == file->current_block->line_index) ? "(L)" : "   "));
                for(unsigned ch = 0; ch < strlen(current->lines[current_line].content); ch ++){
                    if(current == file->current_block->pointer_block && current_line == file->current_block->line_index && ch == file->current_block->ch_index){
                        printf("[%c]", current->lines[current_line].content[ch]);
                        continue;
                    }
                    printf("%c", current->lines[current_line].content[ch]);
                }
                printf("\n");
            }
        }
        current = current->next;
        cont++;
    }
    
    //printf("---------------------\n");
    printf("__________________________________________________________\n");
}
