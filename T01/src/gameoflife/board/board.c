#include <stdlib.h>

#include "board.h"

Board *board_init(char **cells, int n, int A, int B, int C, int size)
{
    Board *board = malloc(sizeof(Board));
    board->values = malloc(size * size * sizeof(int));

    board->iterations = 0;
    board->cell_counts = 0;
    board->size = size;
    board->A = A;
    board->B = B;
    board->C = C;

    return board;
}

static int board_count_next_cells(Board *board, int i, int j)
{
    int delta_i[] = {1, 1, 1, -1, -1, -1, 0, 0};
    int delta_j[] = {1, -1, 0, 1, -1, 0, 1, -1};

    int count = 0;
    for (int i = 0; i < 8; i++)
        count += board->values[delta_i[i]][delta_j[i]];

    return count;
}

int board_iterate_once(Board *board)
{
    int size = board->size;
    int **new_values = malloc(size * size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int c = board_count_next_cells(board, i, j);
            new_values[i][j] = (c == board->A) || (board->A && (board->B <= c && c <= board->C));
        }
    }
    free(board->values);
    board->values = new_values;
    return 0;
}

void board_destroy(Board *board)
{
    free(board->values);
    free(board);
}
