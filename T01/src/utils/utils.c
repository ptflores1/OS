#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char** split(char *string, char* delim, int *n){
    int c = 1;
    for (int i = 0; i < strlen(string); i++) if(string[i] == delim[0]) c++;
    (*n) = c;

    char *token;
    token = strtok(string, delim);

    int it = 0;
    char **list = malloc(c * sizeof(char *));
    while (token != NULL)
    {
        list[it] = strdup(token);
        token = strtok(NULL, delim);
        it++;
    }
    return list;
}