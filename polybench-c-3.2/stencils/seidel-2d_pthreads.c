#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <polybench.h>
#include <pthread.h>
#include "seidel-2d.h"

int NUM_THREADS=0;

int size;
int tsteps;

pthread_barrier_t barrier;


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

/* Matrix Copying */
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
//VERIFICAR
  for (i = start; i < end; i++)
    for (j = 0; j < N; j++)
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

void *kernel_seidel_2d_parallel(void *arg)
{
    int id = *(int *)arg;
    int start_i = (id*(N-2)/NUM_THREADS)+1;
    int end_i = (id+1)*(N-2)/NUM_THREADS;

    for (int t = 0; t <= TSTEPS - 1; t++) {
        for (int i = start_i; i <= end_i; i++) {
            for (int j = 1; j <= N - 2; j++) {
                MATRIX_AUX[i][j] = (MATRIX[i-1][j-1] + MATRIX[i-1][j] + MATRIX[i-1][j+1]
                       + MATRIX[i][j-1] + MATRIX[i][j] + MATRIX[i][j+1]
                       + MATRIX[i+1][j-1] + MATRIX[i+1][j] + MATRIX[i+1][j+1])/9;
            }
        }
        copy_array_line(start_i, end_i, MATRIX, MATRIX_AUX);
        // Sync Threads
        pthread_barrier_wait(&barrier);
    }

}

void aloc_matrix()
{
  MATRIX = (DATA_TYPE **)malloc(sizeof(DATA_TYPE)*size);
  for(int i=0; i<size; i++){
    MATRIX[i] = (DATA_TYPE *)malloc(sizeof(DATA_TYPE)*size);
  }

  MATRIX_AUX = (DATA_TYPE **)malloc(sizeof(DATA_TYPE)*size);
  for(int i=0; i<size; i++){
    MATRIX_AUX[i] = (DATA_TYPE *)malloc(sizeof(DATA_TYPE)*size);
  }
}

void liberarMatrizes(){
  for(int i=0; i<size; i++){
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

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-np") == 0 && i + 1 < argc) {
        NUM_THREADS = atoi(argv[i + 1]);
        break;
    }
  }
  if (np != 0) {
    printf("O valor de -np é %s\n", np);
  } else {
    printf("O argumento -np não foi encontrado\n");
  }

  pthread_t threads[NUM_THREADS];

  aloc_matrix();

  /* Initialize array(s). */
  init_array (MATRIX);
  
  /* Copying array(s). */
  copy_array (MATRIX, MATRIX_AUX);
  

  /* Start timer. */
  polybench_start_instruments;


  int thread_ids[NUM_THREADS];
    

  // Inicialização da barreira
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);

  // Criação das threads
  for (int i = 0; i < NUM_THREADS; i++) {
      thread_ids[i] = i;
      pthread_create(&threads[i], NULL, kernel_seidel_2d_parallel, &thread_ids[i]);
  }

  // Aguarda a conclusão das threads
  for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
  }

  // Libera a memória da barreira
  pthread_barrier_destroy(&barrier);

  liberarMatrizes();

  //kernel_seidel_2d (tsteps, n, POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(size, MATRIX));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(MATRIX);

  return 0;
}