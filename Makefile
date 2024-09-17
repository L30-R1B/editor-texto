# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g

# Nome do executável
TARGET = main

# Diretório de saída para os arquivos objeto
OBJDIR = obj

# Lista de arquivos fonte
SOURCES = src/main.c src/save.c src/open.c src/edit.c src/file.c src/stack.c src/file_editor.c
# Lista de cabeçalhos
HEADERS = include/save.h include/open.h include/edit.h include/file.h include/stack.h include/file_editor.h

# Gera os nomes dos arquivos objeto
OBJECTS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Regra principal para construir o executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Regra para construir arquivos objeto a partir de arquivos fonte
$(OBJDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(OBJECTS) $(TARGET)

# Crie o diretório de objetos se não existir
$(shell mkdir -p $(OBJDIR))