#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <polybench.h>
#include "seidel-2d.h"


/* Array initialization. */
static
void init_array (int n,
		 DATA_TYPE POLYBENCH_2D(A,N,N,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / n;
}

/* Matrix Copying */
static
void copy_array (int n,
		 DATA_TYPE POLYBENCH_2D(A,N,N,n,n), DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      B[i][j]=A[i][j];
}

static
void copy_array_line (int start, int end,
		 DATA_TYPE POLYBENCH_2D(A,N,N,n,n), DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int i, j;
//VERIFICAR
  for (i = start; i < end; i++)
    for (j = 0; j < n; j++)
      B[i][j]=A[i][j];
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_2D(A,N,N,n,n))

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, A[i][j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_seidel_2d(int tsteps,
		      int n,
		      DATA_TYPE POLYBENCH_2D(A,N,N,n,n))
{
  int t, i, j;

#pragma scop
  for (t = 0; t <= _PB_TSTEPS - 1; t++)
    for (i = 1; i<= _PB_N - 2; i++)
      for (j = 1; j <= _PB_N - 2; j++)
	A[i][j] = (A[i-1][j-1] + A[i-1][j] + A[i-1][j+1]
		   + A[i][j-1] + A[i][j] + A[i][j+1]
		   + A[i+1][j-1] + A[i+1][j] + A[i+1][j+1])/9.0;
#pragma endscop

}

static
void kernel_seidel_2d_parallel(int tsteps,
		      int n,
		      DATA_TYPE POLYBENCH_2D(A,N,N,n,n), DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
    int id = *(int *)arg;
    int start_i = (id*(N-2)/num_threads)+1;
    int end_i = (id+1)*(N-2)/num_threads;

    for (int t = 0; t <= TSTEPS - 1; t++) {
        for (int i = start_i; i <= end_i; i++) {
            for (int j = 1; j <= N - 2; j++) {
                B[i][j] = (A[i-1][j-1] + A[i-1][j] + A[i-1][j+1]
                       + A[i][j-1] + A[i][j] + A[i][j+1]
                       + A[i+1][j-1] + A[i+1][j] + A[i+1][j+1])/SCALAR_VAL(9.0);
            }
        }
        copy_array_line(start_i, end_i, POLYBENCH_ARRAY(A),POLYBENCH_ARRAY(B));
        // Sync Threads
        pthread_barrier_wait(&barrier);
    }

    return NULL;

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(MATRIX, DATA_TYPE, N, N, n, n);

  POLYBENCH_2D_ARRAY_DECL(MATRIX_AUX, DATA_TYPE, N, N, n, n);

  /* Initialize array(s). */
  init_array (n, POLYBENCH_ARRAY(MATRIX));
  
  /* Copying array(s). */
  copy_array (n, POLYBENCH_ARRAY(MATRIX), POLYBENCH_ARRAY(MATRIX_AUX));
  

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
    const int num_threads = 2; // Defina o número de threads

    // Inicialização e configuração das variáveis
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    pthread_barrier_t barrier;

    // Inicialização da barreira
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Criação das threads
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, kernel_seidel_2d_parallel, &thread_ids[i]);
    }

    // Aguarda a conclusão das threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libera a memória da barreira
    pthread_barrier_destroy(&barrier);

  //kernel_seidel_2d (tsteps, n, POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
