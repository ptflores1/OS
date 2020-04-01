typedef struct 
{
    int **values;
    int iterations;
    int cell_counts;
    int size;
    int A, B, C;
} Table;

Table *table_init(int **cells, int n, int size, int A, int B, int C);

int table_iterate_once(Table *table);

void table_destroy(Table *table);
