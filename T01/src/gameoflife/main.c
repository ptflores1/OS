#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "board/board.h"
#include "../utils/utils.h"

int process_line;
FILE *file;
char *line;
int list_size;
char **list;
Board *board;

void write_info(Board *board, int is_sigint, int finished);

void handle_sigint(int signal)
{
    printf("%d CATCHED SIGINT\n", getpid());
    write_info(board, 1, 0);
    exit(0);
}

void cleanup()
{
    printf("%d CLEANUP...\n", getpid());
    if (file) fclose(file);
    if(line) free(line);
    if(list){
        for (int i = 0; i < list_size; i++) if (list[i]) free(list[i]);
        free(list);
    }
    board_destroy(board);
}

void read_board(int board_index){
    file = fopen("Inputs T01/simulador/tableros.txt", "r");
    if(!file){
        printf("%d FILE: %s DOES NOT EXIST\n", getpid(), "Inputs T01/simulador/tableros.txt");
        return;
    }

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

void write_info(Board* board, int is_sigint, int finished){
    printf("%d WRITING BOARD\n", getpid());
    char msg[8];
    if(is_sigint)
        strcpy(msg, "SIGNAL");
    else if(finished)
        strcpy(msg, "NOTIME");
    else
        strcpy(msg, "NOCELLS");

    char folder[256];
    strcpy(folder, "files");
    struct stat sb;
    if (!(stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)))
        mkdir(folder, 0777);

    char sprocess_line[10];
    
    sprintf(sprocess_line, "%d", process_line);
    strcat(folder, "/");
    strcat(folder, sprocess_line);
    strcat(folder, ".csv");

    FILE *fp = fopen(folder, "w");
    if (!fp)
    {
        printf("%d FILE: %s DOES NOT EXIST\n", getpid(), folder);
        return;
    }
    fprintf(fp, "%d, %d, %s\n", board->cell_counts, board->iterations, msg);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    atexit(cleanup);
    signal(SIGINT, handle_sigint);
    if (argc != 8)
    {
        printf("%d Cantidad de argumentos incorrecta.\n", getpid());
        printf("%d Modo de uso: gameoflife iteraciones A B C D tablero proceso.\n", getpid());
        exit(0);
    }
    printf("%d CALLED gameoflife %s %s %s %s %s %s %s\n", getpid(), argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
    int iters = atoi(argv[1]);
    int A = atoi(argv[2]);
    int B = atoi(argv[3]);
    int C = atoi(argv[4]);
    int D = atoi(argv[5]);
    int board_index = atoi(argv[6]);
    process_line = atoi(argv[7]);

    read_board(board_index);

    board = board_init(list, A, B, C, D);
    // printf("\n");
    // board_print(board);
    int i;
    for (i = 0; i < iters; i++) if (!board_iterate_once(board)) break;
    
    if (i == iters - 1)
        write_info(board, 0, 1);
    else
        write_info(board, 0, 0);

    return 0;
}