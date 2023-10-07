# 2d-seidel-computation
Work of concurrent programming that consists in parallelizing 2d Seidel algorithm

Alunos:<br>
Arthur Maciel   120119<br>
Gabriel Biscaia 118928<br>
Pedro Zafalon   120117<br>

* * * * * * * * * *
<h1>PolyBench/C 3.2</h1>

Copyright (c) 2011-2012 the Ohio State University.
Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>


<h3>Available benchmarks:</h3>

* stencils/seidel-2d_sequential.c
* stencils/seidel-2d_pthreads.c
* stencils/seidel-2d_mpi.c

<br>
<h3>To compile the code:</h3>

To run the application developed in this project, we must first compile the 'main.c' code. To do this, execute the following command in the terminal:


gcc main.c -o main


After compiling the "main.c" code, you only need to execute `./main` with the necessary arguments. Below, you can see the required arguments:

- Mode: "-m {mpi, pthreads, sequential}"
- Number of Processors: "-np {4, 8, 16}"
- Size: "-s {SMALL, STANDARD, LARGE}"

Example of execution:

./main -m mpi -np 4 -s SMALL

<h3>Makefile</h3>

You also can run the application using a Makefile, first we need to compile the code, to do this, execute the following command in the terminal:

make compile MODE=m NP=n SIZE=s

and then

make run

- m = {mpi, pthreads, sequential}
- n = {number of process that you computer can support}
- s = {SMALL, STANDARD, LARGE} 

<br>

