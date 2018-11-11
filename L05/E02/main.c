#include <stdio.h>
#include <stdlib.h>

int** malloc2dR(int rows, int cols)
{
    int** m = (int**)calloc(rows, sizeof(int*));

    for (int i = 0; i < rows; i++) {
        m[i] = (int*)calloc(cols, sizeof(int));
    }

    return m;
}

void separa(int** m, int rows, int cols, int** blacks, int** whites, int* blacks_len, int* whites_len)
{
    // Print matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", m[i][j]);
        }
        printf("\n");
    }

    // Allocate blacks and whites vectors
    int total_cells = rows * cols;
    *whites_len = (total_cells % 2 == 0) ? (total_cells / 2) : (1 + (total_cells / 2));
    *whites = malloc(*whites_len * sizeof(int));

    *blacks_len = total_cells - *whites_len;
    *blacks = malloc(*blacks_len * sizeof(int));

    int whites_index = 0, blacks_index = 0;

    // Print matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((j + i % 2) % 2 == 0) {
                printf("W\t");
                (*whites)[whites_index++] = m[i][j];
            } else {
                printf("B\t");
                (*blacks)[blacks_index++] = m[i][j];
            }
        }
        printf("\n");
    }

    return;
}

int main()
{
    // Setup
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Couldn't open input.txt");
        return 1;
    }

    // Read matrix dims
    int rows, cols;
    fscanf(fp, "%d %d ", &rows, &cols);
    printf("rows: %d, cols: %d\n", rows, cols);

    int** m = malloc2dR(rows, cols);

    // Read matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(fp, " %d ", &m[i][j]);
        }
    }

    // Vectors of ints which contain the values of the matrix interpeted as a chessboard
    int *blacks, *whites;
    int blacks_len, whites_len;

    separa(m, rows, cols, &blacks, &whites, &blacks_len, &whites_len);

    printf("Whites\n");
    for (int i = 0; i < whites_len; i++) {
        printf("%d ", whites[i]);
    }
    printf("\n");
    printf("Blacks\n");
    for (int i = 0; i < blacks_len; i++) {
        printf("%d ", blacks[i]);
    }
    printf("\n");

    free(blacks);
    free(whites);

    // Cleanup
    for (int i = 0; i < rows; i++) {
        free(m[i]);
    }
    fclose(fp);

    return 0;
}
