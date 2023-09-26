#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int opt;
    char *mode = NULL;
    int np = 0;
    char *size = NULL;

    // Define as opções de linha de comando
    while ((opt = getopt(argc, argv, "m:np:s:")) != -1) {
        switch (opt) {
            case 'm':
                mode = optarg;
                break;
            case 'n':
                np = atoi(optarg);
                break;
            case 's':
                size = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s -m [modo] -n [np] -s [size]\n", argv[0]);
                return 1;
        }
    }

    // Verifica se todas as opções obrigatórias foram especificadas
    if (mode == NULL || np == 0 || size == NULL) {
        fprintf(stderr, "Você deve especificar o modo (-m), o número de processadores (-n) e o tamanho (-s).\n");
        return 1;
    }

    // Exibe os valores especificados
    printf("Modo: %s\n", mode);
    printf("Número de Processadores: %d\n", np);
    printf("Tamanho: %s\n", size);


    if ((strcmp(mode, "pthreads") == 0) || (strcmp(mode, "sequencial") == 0) || (strcmp(mode, "mpi") == 0)){
            char comando_compile[900];
            char comando_exec[100];         
            
            if(strcmp(mode, "pthreads") == 0){
                comando_compile = "gcc -I utilities -I stencils/seidel-2d utilities/polybench.c stencils/seidel-2d/seidel-2d_pthreads.c -o seidel-2d_pthreads -DPOLYBENCH_TIME -D_DATASET";
                comando_exec="./seidel-2d_pthreads";
                sprintf(comando_exec, "./seidel-2d_pthreads -np %d", np);
            }
            else if(strcmp(mode, "sequencial") == 0){
                comando_compile = "gcc -I utilities -I stencils/seidel-2d utilities/polybench.c stencils/seidel-2d/seidel-2d_sequential.c -o seidel-2d_sequential -DPOLYBENCH_TIME -D_DATASET";
                comando_exec="./seidel-2d";
            }
            else if((strcmp(mode, "mpi") == 0)){
                comando_compile = "mpicc -I utilities -I stencils utilities/polybench.c stencils/seidel-2d_mpi.c -o seidel-2d_mpi -DPOLYBENCH_TIME -D_DATASET";
                sprintf(comando_exec, "mpirun -np %d ./sequential-2d_mpi.c", np);
            }
            
            // Encontra a posição de "DATASET" na string
            char *pos = strstr(comando_compile, "_DATASET");

            if (pos != NULL) {
                // Verifica se np é válido
                if (strcmp(np, "LARGE") == 0 || strcmp(np, "STANDARD") == 0 || strcmp(np, "SMALL") == 0) {
                    // Calcula o tamanho da parte do comando após "DATASET"
                    size_t tamanho_restante = strlen(pos);

                    // Move o restante do comando para a direita para abrir espaço para np
                    memmove(pos + strlen(np), pos, tamanho_restante + 1);

                    // Copia np para a posição vazia
                    strncpy(pos, np, strlen(np));
                } else {
                    printf("Valor de np inválido.\n");
                }
            } else {
                printf("Texto 'DATASET' não encontrado na string.\n");
            }


            int status = system(comando_compile);
            if(status ==0){
                system(comando_exec);
            } else {
                printf("Erro ao executar o comando 'gcc'.\n");
                return 1; // Retorna um código de erro
            }

    } else {
        printf("Modo não reconhecido\n");
    }


    return 0;
}