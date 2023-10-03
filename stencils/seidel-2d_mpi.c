#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <polybench.h>
#include "seidel-2d.h"
#include <mpi.h>

int size;
int tsteps;
int rank, num_procs;
MPI_Datatype COLUMN;
MPI_Status status;

DATA_TYPE **MATRIX;
DATA_TYPE **MATRIX_AUX;

static
void init_array (DATA_TYPE **A)
{
    int i, j;

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / size;
}

static
void copy_array (DATA_TYPE **A, DATA_TYPE **B)
{
    int i, j;

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            B[i][j]=A[i][j];
}

static
void copy_array_line (int start, int end, DATA_TYPE **A, DATA_TYPE **B)
{
    int i, j;

    for (i = start; i < end; i++)
        for (j = 0; j < size; j++)
            B[i][j]=A[i][j];
}

static
void print_array(int n, DATA_TYPE **A)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            fprintf(stderr, DATA_PRINTF_MODIFIER, A[i][j]);
            if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
        }
    fprintf(stderr, "\n");
}

void kernel_seidel_2d_parallel(int start_i, int end_i)
{
    for (int t = 0; t <= tsteps - 1; t++) {
        for (int i = start_i; i <= end_i; i++) {
            for (int j = 1; j <= size - 2; j++) {
                MATRIX_AUX[i][j] = (MATRIX[i-1][j-1] + MATRIX[i-1][j] + MATRIX[i-1][j+1]
                                    + MATRIX[i][j-1] + MATRIX[i][j] + MATRIX[i][j+1]
                                    + MATRIX[i+1][j-1] + MATRIX[i+1][j] + MATRIX[i+1][j+1]) / 9;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        copy_array_line(start_i, end_i, MATRIX_AUX, MATRIX);
    }
}

void aloc_matrix()
{
    MATRIX = (DATA_TYPE **)malloc(sizeof(DATA_TYPE *) * size);
    for (int i = 0; i < size; i++) {
        MATRIX[i] = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * size);
    }

    MATRIX_AUX = (DATA_TYPE **)malloc(sizeof(DATA_TYPE *) * size);
    for (int i = 0; i < size; i++) {
        MATRIX_AUX[i] = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * size);
    }
}

void liberarMatrizes()
{
    for (int i = 0; i < size; i++) {
        free(MATRIX[i]);
        free(MATRIX_AUX[i]);
    }

    free(MATRIX);
    free(MATRIX_AUX);
}

int main(int argc, char** argv)
{
    /* Retrieve problem size. */
    size = N;
    tsteps = TSTEPS;

    aloc_matrix();
    init_array(MATRIX);
    copy_array(MATRIX, MATRIX_AUX);

    polybench_start_instruments;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    /* Compute the range of rows for each process */ //não sei o que faz
    int rows_per_proc = (size - 2) / num_procs;
    int start_i = rank * rows_per_proc + 1;
    int end_i = (rank == num_procs - 1) ? size - 2 : start_i + rows_per_proc - 1;

    /* Start timer. */
    double start_time = MPI_Wtime();

    kernel_seidel_2d_parallel(start_i, end_i);

    /* Stop timer and print time taken */ //Acho que pode tirar
    double end_time = MPI_Wtime();
    if(rank == 0) {
        printf("Time: %f seconds\n", end_time - start_time);
    }

    /* Free memory and finalize MPI */
    liberarMatrizes();
    MPI_Finalize();

    polybench_stop_instruments;
    polybench_print_instruments;

    polybench_prevent_dce(print_array(size, MATRIX));

    return 0;
}