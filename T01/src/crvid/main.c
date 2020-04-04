#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../utils/utils.h"

FILE *file;
char *line;
char **list;
int list_size;
int *children;
int *children_processes;
int children_size;
int child;
int current_process;

void collect_data();

void handle_sigint(int signal)
{
    if(children_size != 0){
        for (int i = 0; i < children_size; i++)
            kill(children[i], SIGINT);

        while (children_size--)
            wait(NULL);
    }
    
    if(child != -1){
         kill(child, SIGINT);
         wait(NULL);
    }
    exit(0);
}

void cleanup()
{
    collect_data();
    printf("%d CLEANUP...\n", getpid());
    if (file) fclose(file);
    if (line) free(line);
    if (list)
    {
        for (int i = 0; i < list_size; i++)
            free(list[i]);
        free(list);
    }
    if (children) free(children);
    if (children_processes)free(children_processes);
}

static void read_process(char * filename, int board_index)
{
    file = fopen(filename, "r");

    line = NULL;
    size_t len = 0;
    ssize_t read;
    int it = 0;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        if (it == board_index)
        {
            char *s = " ";
            list = split(line, s, &list_size);
        }
        it++;
    }
}

void collect_data(){
    if(children_size != 0){
        printf("%d Collecting multiple\n", getpid());

        char process_path[256];
        sprintf(process_path, "files/%d.csv", current_process);
        FILE *process_file = fopen(process_path, "w");
        for (int i = 0; i < children_size; i++)
        {
            printf("Process %d reading from %d\n", current_process, children_processes[i]);
            char child_path[256];
            sprintf(child_path, "files/%d.csv", children_processes[i]);
            FILE *child_file = fopen(child_path, "r");
        
            char ch;
            while ((ch = fgetc(child_file)) != EOF)  fputc(ch, process_file);

            fclose(child_file);
        }
        fclose(process_file);
    } 
}

int main(int argc, char *argv[])
{
    atexit(cleanup);
    signal(SIGINT, handle_sigint);
    child = -1;
    sleep(1);
    if (argc != 3)
    {
        printf("Cantidad de argumentos incorrecta.\n");
        printf("Modo de uso: ./crvid input line.\n");
        exit(0);
    }
    printf("%d CALLED %s %s %s\n",getpid(), argv[0], argv[1], argv[2]);
    char *filename = argv[1];
    int line_idx = atoi(argv[2]);
    current_process = atoi(argv[2]);
    read_process(filename, line_idx);

    if (strcmp(list[0], "0") == 0) // Generador
    { 
    children_size = atoi(list[2]);
    children = malloc(children_size * sizeof(int));
    children_processes = malloc(children_size * sizeof(int));
    for (int i = 0; i < children_size; i++)
    {
        children_processes[i] = atoi(list[i + 3]);
        if ((children[i] = fork()) == 0)
        {
            char *args[] = {"./crvid", argv[1], list[i + 3], NULL};
            execvp(args[0], args);
        }
        else if (children[i] < 0)
            printf("%d No se pudo generar el proceso %s", getpid(), list[i + 3]);
        else
        {
            int n = atoi(list[2]);
            while (n--)
                wait(NULL);
        }
    }
    }
    else
    {
        if ((child = fork()) == 0)
        {
            char *args[] = {"./gameoflife", list[1], list[2], list[3], list[4], list[5], list[6], argv[2], NULL};
            execvp(args[0], args);
        }
        else if (child < 0) printf("%d No se pudo generar el proceso %s",getpid(), argv[2]);
        else wait(NULL);
    }

    return 0;
}