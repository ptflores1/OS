#include <stdio.h>
#include <stdlib.h>

#include "board/board.h"
#include "../utils/utils.h"

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Cantidad de argumentos incorrecta.\n");
        printf("Modo de uso: gameoflife iteraciones A B C D tablero.\n");
    }

    FILE * file = fopen("boards.txt", "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if(line[read-1] == '\n') line[read - 1] = '\0';
        int size;
        char *s = " ";
        char**list = split(line, s, &size);
        for (int i = 0; i < size; i++) printf("%s ", list[i]);
        printf("\n");
        for (int i = 0; i < size; i++) free(list[i]);
        free(list);
    }
    if (line)
        free(line);
    fclose(file);
    return 0;
    }