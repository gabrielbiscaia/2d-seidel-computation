#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <polybench.h>
#include "seidel-2d.h"

// Inicializa a matriz
static void init_array(int n,
                       DATA_TYPE POLYBENCH_2D(A, N, N, n, n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      A[i][j] = ((DATA_TYPE)i * (j + 2) + 2) / n;
}

// Função SEIDEL propriamente dita
static void kernel_seidel_2d(int tsteps,
                             int n,
                             DATA_TYPE POLYBENCH_2D(A, N, N, n, n))
{
  int t, i, j;

#pragma scop
  for (t = 0; t <= _PB_TSTEPS - 1; t++)
    for (i = 1; i <= _PB_N - 2; i++)
      for (j = 1; j <= _PB_N - 2; j++)
        A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
#pragma endscop
}

int main(int argc, char **argv)
{
  int n = N;
  int tsteps = TSTEPS;

  // Declara a matriz
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);

  // Iniciliza a matriz
  init_array(n, POLYBENCH_ARRAY(A));

  // Inicia o tempo
  polybench_start_instruments;

  // Começa a fazer as operações
  kernel_seidel_2d(tsteps, n, POLYBENCH_ARRAY(A));

  // Para o tempo e exibe-o
  polybench_stop_instruments;
  polybench_print_instruments;

  // Previne "dead-code elimination".
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));

  // Libera a memória destinada a matriz
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
