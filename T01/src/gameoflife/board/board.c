#include <stdlib.h>
#include <stdio.h>

#include "board.h"

Board *board_init(char **cells, int A, int B, int C, int size)
{
    Board *board = malloc(sizeof(Board));
    board->values = calloc(size, sizeof(int*));
    for (int i = 0; i < size; i++) board->values[i] = calloc(size, sizeof(int));

    board->iterations = 0;
    board->cell_counts = 0;
    board->size = size;
    board->A = A;
    board->B = B;
    board->C = C;

    for (int i = 0; i < atoi(cells[0]); i++)
        board->values[atoi(cells[1 + 2 * i])][atoi(cells[2 + 2 * i])] = 1;

    return board;
}

static int board_count_next_cells(Board *board, int i, int j)
{
    int delta_i[] = {1, 1, 1, -1, -1, -1, 0, 0};
    int delta_j[] = {1, -1, 0, 1, -1, 0, 1, -1};

    int count = 0;
    for (int k = 0; k < 8; k++){
        int ii = delta_i[k] + i;
        int jj = delta_j[k] + j;
        
        if (ii >= 0 && ii < board->size && jj >= 0 && jj < board->size)
            count += board->values[ii][jj];
    }
    return count;
}

int board_iterate_once(Board *board)
{
    int size = board->size;
    int **new_values = calloc(size, sizeof(int *));
    for (int i = 0; i < size; i++) new_values[i] = calloc(size, sizeof(int));

    int total = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int c = board_count_next_cells(board, i, j);
            new_values[i][j] = ((c == board->A) || (board->values[i][j] && (board->B <= c && c <= board->C)));
            total += new_values[i][j];
        }
    }

    if(board->values){
        for (int i = 0; i < size; i++) if (board->values[i]) free(board->values[i]);
        free(board->values);
    } 
    board->values = new_values;
    board->cell_counts = total;
    board->iterations++;
    // printf("BOARD: \n");
    // board_print(board);
    return total;
}

void board_print(Board * board){
    for (int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->size; j++) printf("%d ", board->values[i][j]);
        printf("\n");   
    }
}

void board_destroy(Board *board)
{
    if(board){
        if (board->values)
        {
            for (int i = 0; i < board->size; i++) if (board->values[i]) free(board->values[i]);
            free(board->values);
        }
        free(board);
    }
    
}
