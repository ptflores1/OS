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

void at_timeout(int signal) {
    printf("%d TIME OUT\n", getpid());
    kill(getpid(), SIGINT);
}

void handle_sigint(int signal)
{
    printf("%d CATCHED SIGINT\n", getpid());
    int n_children = children_size;
    if (n_children != 0)
    {
        for (int i = 0; i < n_children; i++)
            kill(children[i], SIGINT);

        while (n_children--)
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
        for (int i = 0; i < list_size; i++) if(list[i]) free(list[i]);
        free(list);
    }
    if (children) free(children);
    if (children_processes) free(children_processes);
}

static void read_process(char * filename, int board_index)
{
    file = fopen(filename, "r");
    if (!file){
        printf("%d FILE: %s DOES NOT EXIST\n", getpid(), filename);
        return;
    }
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
    printf("%d children_size: %d\n", getpid(), children_size);
    if (children_size > 0)
    {
        printf("%d COLLECTING MULTIPLE\n", getpid());

        char process_path[256];
        sprintf(process_path, "files/%d.csv", current_process);
        FILE *process_file = fopen(process_path, "w");
        if (!process_file)
        {
            printf("%d FILE: %s DOES NOT EXIST\n", getpid(), process_path);
            return;
        }
        for (int i = 0; i < children_size; i++)
        {
            printf("Process %d reading from %d\n", current_process, children_processes[i]);
            char child_path[256];
            sprintf(child_path, "files/%d.csv", children_processes[i]);
            FILE *child_file = fopen(child_path, "r");
            if (!child_file)
            {
                printf("%d FILE: %s DOES NOT EXIST\n", getpid(), child_path);
                return;
            }

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
    signal(SIGALRM, at_timeout);
    child = -1;
    if (argc != 3)
    {
        printf("%d Cantidad de argumentos incorrecta.\n", getpid());
        printf("%d Modo de uso: ./crvid input line.\n", getpid());
        exit(0);
    }
    printf("%d CALLED %s %s %s\n",getpid(), argv[0], argv[1], argv[2]);
    char *filename = argv[1];
    int line_idx = atoi(argv[2]);
    current_process = atoi(argv[2]);
    read_process(filename, line_idx);

    if (strcmp(list[0], "0") == 0) // Generador
    {
        alarm(atoi(list[1]));

        children_size = 0;
        children = malloc(atoi(list[2]) * sizeof(int));
        children_processes = malloc(atoi(list[2]) * sizeof(int));
        for (int i = 0; i < atoi(list[2]); i++)
        {
            children_processes[i] = atoi(list[i + 3]);
            if ((children[i] = fork()) == 0)
            {
                char *args[] = {"./crvid", argv[1], list[i + 3], NULL};
                execvp(args[0], args);
            }
            else if (children[i] < 0)
                printf("%d No se pudo generar el proceso %s", getpid(), list[i + 3]);
            else children_size++;
        }
        printf("%d CHILDREN SIZE: %d\n", getpid(), children_size);
        int n = children_size;
        while (n--) wait(NULL);
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