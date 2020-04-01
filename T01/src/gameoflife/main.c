#include <stdio.h>
#include <stdlib.h>

#include "board/board.h"
#include "../utils/utils.h"

FILE *file;
char *line;
int list_size;
char **list;

void cleanup(){
    if(file) fclose(file);
    if(line) free(line);
    if(list){
        for (int i = 0; i < list_size; i++) free(list[i]);
        free(list);
    }
}

void read_board(int board_index){
    file = fopen("boards.txt", "r");

    line = NULL;
    size_t len = 0;
    ssize_t read;
    int it = 0;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        if (it == board_index)
        {
            char *s = " ";
            list = split(line, s, &list_size);
        }
        it++;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Cantidad de argumentos incorrecta.\n");
        printf("Modo de uso: gameoflife iteraciones A B C D tablero.\n");
        exit(0);
    }

    atexit(cleanup);
    int iters = atoi(argv[1]);
    int A = atoi(argv[2]);
    int B = atoi(argv[3]);
    int C = atoi(argv[4]);
    int D = atoi(argv[5]);
    int board_index = atoi(argv[6]);

    read_board(board_index);
    
    for (int i = 0; i < list_size; i++) printf("%s ", list[i]);
    printf("\n");
    
    return 0;
}