#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int opt;
    char mode[50] = "";
    int np = 0;
    char size[50] = "";
    


    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            strcpy(mode, argv[i+1]);
        }
        else if(strcmp(argv[i], "-np") == 0 && i + 1 < argc){
            np = atoi(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-s") == 0 && i + 1 < argc){
            strcpy(size, argv[i+1]);
        }
    }

    // Verifica se todas as opções obrigatórias foram especificadas
    if ((strcmp(mode, "")==0) || np == 0 || (strcmp(size, "")==0)) {
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
            
            if (strcmp(mode, "pthreads") == 0) {
                strcpy(comando_compile, "gcc -I utilities -I stencils/seidel-2d utilities/polybench.c stencils/seidel-2d/seidel-2d_pthreads.c -o executables/seidel-2d_pthreads -DPOLYBENCH_TIME -D_DATASET");
                sprintf(comando_exec, "./executables/seidel-2d_pthreads -np %d", np);
            }
            else if (strcmp(mode, "sequencial") == 0) {
                strcpy(comando_compile, "gcc -I utilities -I stencils/seidel-2d utilities/polybench.c stencils/seidel-2d/seidel-2d_sequential.c -o executables/seidel-2d_sequential -DPOLYBENCH_TIME -D_DATASET");
                strcpy(comando_exec, "./executables/seidel-2d_sequential");
            }
            else if (strcmp(mode, "mpi") == 0) {
                strcpy(comando_compile, "mpicc -I utilities -I stencils utilities/polybench.c stencils/seidel-2d_mpi.c -o executables/seidel-2d_mpi -DPOLYBENCH_TIME -D_DATASET");
                sprintf(comando_exec, "mpirun -np %d ./executables/seidel-2d_mpi", np);
            }
            
            // Encontra a posição de "DATASET" na string
            char *pos = strstr(comando_compile, "_DATASET");

            if (pos != NULL) {
                // Verifica se np é válido
                if (strcmp(size, "LARGE") == 0 || strcmp(size, "STANDARD") == 0 || strcmp(size, "SMALL") == 0) {
                    // Calcula o tamanho da parte do comando após "DATASET"
                    size_t tamanho_restante = strlen(pos);

                    // Move o restante do comando para a direita para abrir espaço para size
                    memmove(pos + strlen(size), pos, tamanho_restante + 1);

                    // Copia size para a posição vazia
                    strncpy(pos, size, strlen(size));
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
