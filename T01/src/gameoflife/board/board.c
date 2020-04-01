#include <stdlib.h>

#include "board.h"

Table *table_init(int **cells, int n, int size, int A, int B, int C)
{
    Table *table = malloc(sizeof(Table));
    table->values = malloc(size * size * sizeof(int));

    table->iterations = 0;
    table->cell_counts = 0;
    table->size = size;
    table->A = A;
    table->B = B;
    table->C = C;

    return table;
}

int table_count_next_cells(Table *table, int i, int j){
    int delta_i[] = {1, 1, 1, -1, -1, -1, 0, 0};
    int delta_j[] = {1, -1, 0, 1, -1, 0, 1, -1};

    int count = 0;
    for (int i = 0; i < 8; i++)
        count += table->values[delta_i[i]][delta_j[i]];

    return count;
}

int table_iterate_once(Table *table)
{
    int size = table->size;
    int **new_values = malloc(size * size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int c = table_count_next_cells(table, i, j);
            new_values[i][j] = (c == table->A) || (table->A && (table->B <= c && c <= table->C));
        }
    }
    free(table->values);
    table->values = new_values;
    return 0;
}

void table_destroy(Table *table)
{
    free(table->values);
    free(table);
}
