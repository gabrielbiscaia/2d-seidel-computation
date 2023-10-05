# Variáveis do Makefile
CC = gcc
MPICC = mpicc
CFLAGS = -I utilities -I stencils
CFLAGS_POLYBENCH = -DPOLYBENCH_TIME -D_DATASET
EXECUTABLES_DIR = executables

# Parâmetros do programa
MODE ?= sequential
NP ?= 1
SIZE ?= STANDARD

.PHONY: all compile run clean

all: compile run

compile:
    @mkdir -p $(EXECUTABLES_DIR)
    @if [ "$(MODE)" = "pthreads" ]; then \
        $(CC) $(CFLAGS) $(CFLAGS_POLYBENCH) utilities/polybench.c stencils/seidel-2d_pthreads.c -o $(EXECUTABLES_DIR)/seidel-2d_pthreads -D_DATASET_$(SIZE); \
    elif [ "$(MODE)" = "sequential" ]; then \
        $(CC) $(CFLAGS) $(CFLAGS_POLYBENCH) utilities/polybench.c stencils/seidel-2d_sequential.c -o $(EXECUTABLES_DIR)/seidel-2d_sequential; \
    elif [ "$(MODE)" = "mpi" ]; then \
        $(MPICC) $(CFLAGS) $(CFLAGS_POLYBENCH) utilities/polybench.c stencils/seidel-2d_mpi.c -o $(EXECUTABLES_DIR)/seidel-2d_mpi -D_DATASET_$(SIZE); \
    else \
        echo "Modo não reconhecido"; \
        exit 1; \
    fi

run:
    @if [ "$(MODE)" = "pthreads" ]; then \
        ./$(EXECUTABLES_DIR)/seidel-2d_pthreads -np $(NP); \
    elif [ "$(MODE)" = "sequential" ]; then \
        ./$(EXECUTABLES_DIR)/seidel-2d_sequential; \
    elif [ "$(MODE)" = "mpi" ]; then \
        mpirun -np $(NP) ./$(EXECUTABLES_DIR)/seidel-2d_mpi; \
    else \
        echo "Modo não reconhecido"; \
        exit 1; \
    fi

clean:
    @rm -rf $(EXECUTABLES_DIR)

