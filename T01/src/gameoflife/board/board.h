typedef struct 
{
    int **values;
    int iterations;
    int cell_counts;
    int size;
    int A, B, C;
} Board;

Board *board_init(char **cells, int n, int A, int B, int C, int size);

int board_iterate_once(Board *board);

void board_destroy(Board *board);
